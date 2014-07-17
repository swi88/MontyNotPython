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
    void listen();

private:
    QTcpServer *server;
    QTcpSocket *socket;
    void sendPicture();
    void takePicture();
    void rotateLeft();
    void rotateRight();

private slots:
    void on_newConnection();
    void on_disconnected();
    void on_readyRead();

signals:

};

#endif // SERVER_H
