#include "session.h"

#include <mosquittopp.h>
#include <iostream>
#include <mutex>

GL_BEGIN_NAMESPACE

namespace {
    const char *g_default_host = "localhost";
    const int g_default_port = 1883;
    const int g_default_keep_alive = 60;

    struct Mosqpp
    {
        Mosqpp()
        {
            mosqpp::lib_init();
        }
        ~Mosqpp()
        {
            mosqpp::lib_cleanup();
        }
    };

    void MosqInit()
    {
        static Mosqpp mosqpp;
    }
}

struct SessionPrivate : public mosqpp::mosquittopp
{
    SessionPrivate(Session *p)
        : host(g_default_host)
        , port(g_default_port)
        , keep_alive(g_default_keep_alive)
        , p(p)
    {}

    std::string host;
    int port;
    int keep_alive;
    Session *p;
    Session::ConnectHandler connect_handler;
    Session::MessageHandler message_handler;
    Session::SubscribeHandler subscribe_handler;
    Session::DisconnectHandler disconnect_handler;

    virtual void on_connect(int rc)
    {
        connect_handler(rc);
    }

    virtual void on_disconnect(int rc)
    {
        disconnect_handler(rc);
    }

    virtual void on_publish(int mid)
    {
    }

    virtual void on_message(const struct mosquitto_message *message)
    {
        message_handler(message->mid, std::string(message->topic), std::string((char*)message->payload, (char*)message->payload + message->payloadlen));
    }

    virtual void on_subscribe(int mid, int qos_count, const int *granted_qos)
    {
        subscribe_handler(mid, qos_count);
    }

    virtual void on_unsubscribe(int mid)
    {}

    virtual void on_log(int level, const char *str)
    {
//        std::cerr << __func__ << level << ", " << str << "\n";
    }

    virtual void on_error()
    {
//        std::cerr << __func__ << "\n";
    }
};


Session::Session()
{
    static std::once_flag flag;
    std::call_once(flag, MosqInit);
    d = std::make_unique<SessionPrivate>(this);
}

Session::~Session()
{
}


std::string Session::host() const
{
    return d->host;
}

void Session::set_host(std::string host)
{
    d->host = host;
}

int Session::port() const
{
    return d->port;
}

void Session::set_port(int port)
{
    d->port = port;
}

int Session::keep_alive() const
{
    return d->keep_alive;
}

void Session::set_keep_alive(int keep_alive)
{
    d->keep_alive = keep_alive;
}

void Session::set_connect_handler(ConnectHandler handler)
{
    d->connect_handler = handler;
}

void Session::set_disconnect_handler(DisconnectHandler handler)
{
    d->disconnect_handler = handler;
}

void Session::set_message_handler(MessageHandler handler)
{
    d->message_handler = handler;
}

void Session::set_subscribe_handler(SubscribeHandler handler)
{
    d->subscribe_handler = handler;
}

Session::Result Session::connect(const std::string &username, const std::string &password)
{
    if (!username.empty() && !password.empty())
        d->username_pw_set(username.data(), password.data());

    int result = d->connect_async(d->host.data(), d->port, d->keep_alive);
    return std::make_tuple(result == MOSQ_ERR_SUCCESS ? Session::SUCCESS : Session::INVALID, 0);
}

Session::Result Session::disconnect()
{
    return std::make_tuple(d->disconnect() == MOSQ_ERR_SUCCESS ? Session::SUCCESS : Session::INVALID, 0);
}

Session::Result Session::start()
{
    return std::make_tuple(d->loop_start() == MOSQ_ERR_SUCCESS ? Session::SUCCESS : Session::INVALID, 0);
}

Session::Result Session::stop()
{
    return std::make_tuple(d->loop_stop() == MOSQ_ERR_SUCCESS ? Session::SUCCESS : Session::INVALID, 0);
}


Session::Result Session::subscribe(const std::string &topic)
{
    int mid;
    int result = d->subscribe(&mid, topic.data(), 1);
    return std::make_tuple(result == MOSQ_ERR_SUCCESS ? Session::SUCCESS : Session::INVALID, mid);
}

Session::Result Session::publish(const std::string &topic, const std::string &message)
{
    int mid;
    int result = d->publish(&mid, topic.data(), message.size(), message.data());
    return std::make_tuple(result == MOSQ_ERR_SUCCESS ? Session::SUCCESS : Session::INVALID, mid);
}


GL_END_NAMESPACE
