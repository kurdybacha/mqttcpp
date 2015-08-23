#ifndef _SESSION_H_
#define _SESSION_H_

#include <global.h>
#include <string>
#include <functional>
#include <tuple>

GL_BEGIN_NAMESPACE

class SessionPrivate;

class Session
{
public:

   enum ResultCode {
       SUCCESS = 0,
       INVALID = 1
   };

   typedef std::function<void(int)>                           ConnectHandler;
   typedef std::function<void(int)>                           DisconnectHandler;
   typedef std::function<void(int, std::string, std::string)> MessageHandler;
   typedef std::function<void(int, int)>                      SubscribeHandler;
   typedef std::tuple<ResultCode, int>                        Result;

   Session();
   virtual ~Session();

   std::string host() const;
   void set_host(std::string host);

   int port() const;
   void set_port(int port);

   int keep_alive() const;
   void set_keep_alive(int keep_alive);

   Result connect(const std::string &username = std::string(), const std::string &password = std::string());
   Result disconnect();

   Result subscribe(const std::string &topic);
   Result publish(const std::string &topic, const std::string &message);

   void set_connect_handler(ConnectHandler handler);
   void set_message_handler(MessageHandler handler) ;
   void set_subscribe_handler(SubscribeHandler handler);
   void set_disconnect_handler(DisconnectHandler handler);

   Result start();
   Result stop();
private:

private:
   GL_DISABLE_COPY(Session)
   GL_DECLARE_PRIVATE(Session)
};

GL_END_NAMESPACE

#endif // _SESSION_H_

