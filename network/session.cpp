#include "session.h"

#include <mosquittopp.h>
#include <iostream>

GL_BEGIN_NAMESPACE

struct SessionPrivate : public mosqpp::mosquittopp
{
    SessionPrivate(std::string host, int port, int keep_alive, Session *p)
        : host(host)
        , port(port)
        , keep_alive(keep_alive)
        , p(p)
    {}

    std::string host;
    int port;
    int keep_alive;
    Session *p;
    Session::ConnectHandler connect_handler;
    Session::MessageHandler message_handler;
    Session::SubscribeHandler subscribe_handler;

    virtual void on_connect(int rc)
    {
        connect_handler(rc);
    }

    virtual void on_disconnect(int rc)
    {
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
//        std::cerr << __func__ << level << str;
    }

    virtual void on_error()
    {
        std::cerr << __func__;
    }
};

void Session::init()
{
    mosqpp::lib_init();
}

Session::Session(std::string host, int port, int keep_alive)
    : d(std::make_unique<SessionPrivate>(host, port, keep_alive, this))
{
}

Session::~Session() = default;

void Session::set_connect_handler(ConnectHandler handler)
{
    d->connect_handler = handler;
}

void Session::set_message_handler(MessageHandler handler)
{
    d->message_handler = handler;
}

void Session::set_subscribe_handler(SubscribeHandler handler)
{
    d->subscribe_handler = handler;
}

void Session::connect(const std::string &username, const std::string &password)
{
    if (!username.empty() && !password.empty())
        d->username_pw_set(username.data(), password.data());
    d->connect_async(d->host.data(), d->port, d->keep_alive);
    d->loop_start();
}

int Session::subscribe(const std::string &topic)
{
    int mid;
    d->subscribe(&mid, topic.data(), 1);
    return mid;
}

int Session::publish(const std::string &topic, const std::string &message)
{
    int mid;
    d->publish(&mid, topic.data(), message.size(), message.data());
    return mid;
}


GL_END_NAMESPACE
