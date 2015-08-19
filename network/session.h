#ifndef _DL_SESSION_H_
#define _DL_SESSION_H_

#include <global.h>

DL_BEGIN_NAMESPACE

class SessionPrivate;

class Session
{
public:
   Session();

   void connect();

private:
   SessionPrivate *d;
};

DL_END_NAMESPACE

#endif // _DL_SESSION_H_

