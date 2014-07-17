#include "server.h"
#include <QCoreApplication>

#include "montycontroller.h"
#include <QDebug>

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    //GPIO* pin17 = new GPIO(17);
    //pin17->export_gpio();
    //pin17->setdir_gpio("out");
    //return 0;
    /**
    Servo servo(0);
    for(int i=50;i<240;i+=5){
	servo.setValue(i);
	sleep(2);
	}
    servo.setValue(60);

    //Stepper stepper(17,10,27,22);
    Stepper stepper(2,3,4,11);
    stepper.clockwise(1000);
    qDebug()<<"started";
    //usleep(10);
    stepper.counterclockwise(1000);
    **/
    MontyController* montyControll = new MontyController();
    return a.exec();
}
