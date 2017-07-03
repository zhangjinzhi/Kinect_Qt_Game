#ifndef __chat_server_h__
#define __chat_server_h__

#include <QTcpServer>

class MySocket;

class ChatServer : public QTcpServer
{
    Q_OBJECT

public:
    ChatServer  ( QObject *parent = NULL );
    ~ChatServer ();

    void    Run ( quint16 port );

protected:
    void    incomingConnection  ( int handle );

private slots:
    void    clientDisconnected  ();

private:
    quint16             _port;
    QList<MySocket*>  _mysockets;
};

#endif // __chat_server_h__
