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
    bool sendPicture(string fileName);

private:
    QTcpServer *server;
    QTcpSocket *socket;


private slots:
    void on_newConnection();
    void on_disconnected();
    void on_readyRead();

signals:
    void takePicture();
    void rotateLeft();
    void rotateRight();
    void zoomIn();
    void zoomOut();
    void armUp();
    void armDown();
};

#endif // SERVER_H
