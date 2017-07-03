#include "chat_server.h"
#include "my_socket.h"
#include <QHostAddress>

ChatServer::ChatServer( QObject *parent /* = NULL */ )
    : QTcpServer( parent )
{
    _mysockets.empty();
}

ChatServer::~ChatServer()
{
    qDeleteAll( _mysockets );
    _mysockets.clear();
}

void ChatServer::Run( quint16 port )
{
    if( !this->listen(QHostAddress::Any, port) )
        printf( "ChatServer listen failed !" );
}

void ChatServer::incomingConnection( int handle )
{
    //printf( "incomingConnection(): %d !\n", handle );
    MySocket *mysocket = new MySocket( this );
    mysocket->setSocketDescriptor( handle );
    connect( mysocket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()) );
    _mysockets.append( mysocket );
}

void ChatServer::clientDisconnected()
{
   // printf( "client disconnected !\n" );
    QList<MySocket*>::iterator iter;
    for( iter = _mysockets.begin(); iter != _mysockets.end(); iter++ ) {
        if( -1 == (*iter)->socketDescriptor() ) {
            _mysockets.erase( iter );
            return;
        }
    }
}
