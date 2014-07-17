#include "montycontroller.h"
#include <QDebug>

MontyController::MontyController(){
    //init wiring pi
    wiringPiSetup();
    ultrasonic = new Ultrasonic();
    stepperRotate = new Stepper(2,3,4,11);
    stepperZoom = new Stepper(17,10,27,22);
    connect(ultrasonic,SIGNAL(receiveDistance(double)),this,SLOT(receiveUltrasonicDistance(double)));

    server = new Server();
    server->listen();
}

void MontyController::receiveUltrasonicDistance(double value)
{
    qDebug()<<value;
}
