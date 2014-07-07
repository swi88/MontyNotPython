#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#ifndef SERVER_H
#define SERVER_H

class Server : public QObject
{
    Q_OBJECT
public:
    Server();



private:
    void listen();
    QTcpServer *server;
    QTcpSocket *socket;


private slots:
    void on_newConnection();
    void on_disconnected();
    void on_readyRead();



};

#endif // SERVER_H
