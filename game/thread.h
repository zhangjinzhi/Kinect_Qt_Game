#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <iostream>
#include "chat_server.h"
//#include "GUI_BubbleWidget.h"

class Thread : public QThread
{
    Q_OBJECT
 public:
     Thread();

     ChatServer server;
     void serverStart();
     //virtual void run(int idx);
     //CBubbleWidget bubble;

 };
#endif
