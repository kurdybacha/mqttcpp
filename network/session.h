#ifndef _SESSION_H_
#define _SESSION_H_

#include <global.h>
#include <string>
#include <functional>

GL_BEGIN_NAMESPACE

class SessionPrivate;

class Session
{
public:
   typedef std::function<void(int)>                           ConnectHandler;
   typedef std::function<void(int, std::string, std::string)> MessageHandler;
   typedef std::function<void(int, int)>                      SubscribeHandler;

   Session(std::string host, int port, int keep_alive);
   ~Session();

   void connect(const std::string &username, const std::string &password);

   int subscribe(const std::string &topic);
   int publish(const std::string &topic, const std::string &message);

   void set_connect_handler(ConnectHandler handler);
   void set_message_handler(MessageHandler handler) ;
   void set_subscribe_handler(SubscribeHandler handler);

   static void init();

private:
   GL_DISABLE_COPY(Session)
   GL_DECLARE_PRIVATE(Session)
};

GL_END_NAMESPACE

#endif // _SESSION_H_

