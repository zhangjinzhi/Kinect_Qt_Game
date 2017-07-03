
#include "thread.h"
#include <QDebug>

 Thread::Thread()
 {
 }

 void Thread::serverStart()
 {
     server.Run( 33333 );
 }
