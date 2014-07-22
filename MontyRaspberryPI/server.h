#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

#ifndef SERVER_H
#define SERVER_H

class Server : public QObject
{
    Q_OBJECT
public:
    Server();
    void listen();
    bool sendPicture(QString fileName);
    void testMethode();

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
    void automaticOff();
    void automaticOn();
};

#endif // SERVER_H
