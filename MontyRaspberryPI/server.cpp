#include "server.h"
#include <cstdio>
#include <QtDebug>

int int_from_bytes(const char * bytes, bool reverse)
{
    // usage: int_from_bytes(bytes, SYSTEM_ENDIANNESS != ARRAY_ENDIANNESS)
    if(!reverse)
        return *(int *)(void *)bytes;

    char tmp[sizeof(int)];

    for(size_t i = sizeof(tmp); i--; ++bytes)
        tmp[i] = *bytes;

    return *(int *)(void *)tmp;
}

Server::Server() :QObject()
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()),
    this, SLOT(on_newConnection()));
}

void Server::listen()
{
    qDebug()<<"listen";
    server->listen(QHostAddress::Any, 5100);
}

void Server::on_newConnection()
{
    socket = server->nextPendingConnection();

    if(socket->state() == QTcpSocket::ConnectedState)
    {
        printf("New connection established.\n");
        qDebug()<<socket->peerAddress();
    }
    connect(socket, SIGNAL(disconnected()),
    this, SLOT(on_disconnected()));
    connect(socket, SIGNAL(readyRead()),
    this, SLOT(on_readyRead()));
}

void Server::on_readyRead()
{   qDebug() << "read input";
    QByteArray tlength = socket->read(4); // read int32
    int length = int_from_bytes(tlength.data(), true);
    QByteArray message = socket->read(length);

    if(strcmp(message.constData(), "#QUIT_COMMAND") == 0)
    {
        socket->disconnectFromHost();
    }
    if(strcmp(message.constData(), "hallo") == 0)
        qDebug() << "hallo";
    printf(">> %s", message.constData());
}

void Server::on_disconnected()
{
    printf("Connection disconnected.\n");
    disconnect(socket, SIGNAL(disconnected()));
    disconnect(socket, SIGNAL(readyRead()));
    socket->deleteLater();
}
