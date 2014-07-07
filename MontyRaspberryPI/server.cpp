#include "server.h"
#include <cstdio>
#include <QtDebug>

Server::Server() :QObject()
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()),
    this, SLOT(on_newConnection()));
}

void Server::listen()
{
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
{
    while(socket->canReadLine())
    {
        QByteArray ba = socket->readLine();

        if(strcmp(ba.constData(), "!exit\n") == 0)
        {
            socket->disconnectFromHost();
            break;
        }
        printf(">> %s", ba.constData());
    }
}

void Server::on_disconnected()
{
    printf("Connection disconnected.\n");
    disconnect(socket, SIGNAL(disconnected()));
    disconnect(socket, SIGNAL(readyRead()));
    socket->deleteLater();
}
