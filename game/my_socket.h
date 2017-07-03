#ifndef __my_socket_h__
#define __my_socket_h__

#include <QThread>
#include <QTcpSocket>
#include <QStringList>

extern QString string;
extern bool isMsg;
class ChatServer;

class MySocket : public QTcpSocket
{
    Q_OBJECT

public:
    MySocket    ( QObject *parent = NULL );
    ~MySocket   ();


private slots:
    void    onReadyRead     ();

};
#endif // __my_socket_h__
