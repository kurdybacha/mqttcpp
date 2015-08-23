#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#include <winsock2.h>
#define snprintf sprintf_s
#endif
#include <iostream>
#include <session.h>
#include <thread>

class SimpleClient
{
public:
    SimpleClient(const std::string &client_id)
        : client_id_(client_id)
    {
        session_.set_connect_handler([&](int rc) -> void {
           if (rc == 0) {
               printf("Connected OK\n");
               mqttcpp::Session::Result result = session_.subscribe(client_id_ + "/#");
               printf("Subscribing to %s (mid=%d)...\n",  client_id_.data(), std::get<0>(result));
           }
        });

        session_.set_disconnect_handler([&](int rc) {
           printf("Disconnected\n");
           cond_var_.notify_one();
        });

        session_.set_message_handler([&](int mid, std::string topic, std::string message) {
            std::size_t id_pos = topic.find_last_of("/") + 1;
            std::string from = topic.substr(id_pos, topic.size() - id_pos);
            printf("[%s] %s\n", from.data(), message.data());
        });

        session_.set_subscribe_handler([&](int mid, int qos_count) {
            printf("Subscribed (mid=%d, qos_count=%d)\n", mid, qos_count);
        });
    }


    ~SimpleClient()
    {
        session_.disconnect();
        {   //wait for disconnect result
            std::unique_lock<std::mutex> lk(mutex_);
            cond_var_.wait(lk);
        }
        session_.stop();
    }

    std::string id() const { return client_id_; }

    mqttcpp::Session::Result publish(const std::string topic, const std::string &message)
    {
        return session_.publish(topic, message);
    }

    mqttcpp::Session::Result connect()
    {
        mqttcpp::Session::Result result = session_.connect();
        result = session_.start();
        return result;
    }

private:
    mqttcpp::Session session_;
    std::string client_id_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <your_id> <friend_id>\n", argv[0]);
        return -1;
    }

    SimpleClient client(argv[1]);

    mqttcpp::Session::Result result = client.connect();

    if (std::get<0>(result) == mqttcpp::Session::INVALID) {
        printf("Connection failed\n");
        return -1;
    }

    std::string friend_id(argv[2]);

    for (std::string message; std::getline(std::cin, message);) {
        if (message == "exit")
            break;
        client.publish(friend_id + "/" + client.id(), message);
    }
}
