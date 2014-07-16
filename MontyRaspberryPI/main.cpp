#include "server.h"

#include <QCoreApplication>
#include <QThread>

#include <hardware/servo.h>
#include <hardware/stepper.h>
#include <unistd.h>
#include <QDebug>

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    // start server
    QThread* t = new QThread;
    Server s;
    // connect server signals to functions
    QObject::connect(&s, SIGNAL(takePicture()), this, SLOT(takePicture);
    s.listen();
    s.moveToThread(t);
    t->start();

    while(1);
    /**
    GPIO* pin17 = new GPIO(17);
    pin17->export_gpio();
    pin17->setdir_gpio("out");
    **/
    return 0;
    //Servo servo(1);
    /**
    Stepper stepper;
    stepper.clockwise(100);
    qDebug()<<"started";
    //usleep(10);
    stepper.counterclockwise(100);
     qDebug()<<"main end";
     **/
    return a.exec();
}

class RemoteListener:
void takePicture()
{
    // todo
}
