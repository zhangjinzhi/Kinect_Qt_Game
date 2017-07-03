
#include "my_socket.h"
#include "chat_server.h"
#include <QTcpSocket>
QString string;
bool isMsg = false;
MySocket::MySocket( QObject *parent /* = NULL */ )
    : QTcpSocket( parent )
{
    connect( this, SIGNAL(readyRead()), this, SLOT(onReadyRead()) );
}

MySocket::~MySocket()
{
    //printf( "~MySocket\n" );
    close();
}

void MySocket::onReadyRead()
{
    isMsg = true;
    char data[1024] = { 0 };
    int len = readData( data, 1024 );
    if( len <= 0 ) {
        //printf( "MySocket::OnReadyRead() read data failed !\n" );
        return;
    }
    string = QString(QLatin1String(data));

   // printf( "receive data: %d\n", data );

//    if( 0 >= writeData(data, len) )
//        printf( "MySocket::OnReadyRead() write data failed !\n" );
//    else
//        printf( "send    data: %s\n", data );
}
