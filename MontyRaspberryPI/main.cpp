#include <QCoreApplication>
#include <QThread>

#include <hardware/servo.h>
#include <hardware/stepper.h>
#include <unistd.h>
#include <QDebug>

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    //Servo servo(1);

    Stepper stepper;
    stepper.clockwise(100);
    qDebug()<<"started";
    //usleep(10);
    stepper.counterclockwise(100);
     qDebug()<<"main end";
    return a.exec();
}
