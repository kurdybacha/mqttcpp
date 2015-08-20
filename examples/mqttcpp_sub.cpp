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

#include <session.h>
#include <iostream>


int main(int argc, char *argv[])
{
    mqttcpp::Session::init();
    mqttcpp::Session session("localhost", 1883, 60);

    session.set_connect_handler([&](int rc) -> void {
       if (rc == 0) {
           printf("Connected OK\n");
           int mid = session.subscribe("chat/test");
           printf("Subscribing to chat/test (mid=%d)...\n", mid);
       }
    });

    session.set_message_handler([&](int mid, std::string topic, std::string message) {
        printf("Message received: (id=%d) (topic=%s) %s\n", mid, topic.data(), message.data());
    });

    session.set_subscribe_handler([&](int mid, int qos_count) {
        printf("Subscribed (mid=%d, qos_count=%d)\n", mid, qos_count);
    });

    session.connect(std::string(), std::string());

    int n;
    std::cin >> n;
}

