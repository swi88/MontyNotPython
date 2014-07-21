#include "montycontroller.h"
#include <QDebug>

MontyController::MontyController(){
    //init wiring pi
    //wiringPiSetup();
    //ultrasonic = new Ultrasonic();
    //stepperRotate = new Stepper(2,3,4,11);
    //stepperZoom = new Stepper(17,10,27,22);
    connect(ultrasonic,SIGNAL(receiveDistance(double)),this,SLOT(receiveUltrasonicDistance(double)));

    server = new Server();
    connect(server,SIGNAL(takePicture()),this,SLOT(takePicture()));
    connect(server,SIGNAL(rotateLeft()),this,SLOT(rotateLeft()));
    connect(server,SIGNAL(rotateRight()),this,SLOT(rotateRight()));
    connect(server,SIGNAL(zoomIn()),this,SLOT(zoomIn()));
    connect(server,SIGNAL(zoomOut()),this,SLOT(zoomOut()));
    connect(server,SIGNAL(armUp()),this,SLOT(armUp()));
    connect(server,SIGNAL(armDown()),this,SLOT(armDown()));
    server->listen();
}

void MontyController::receiveUltrasonicDistance(double value)
{
    qDebug()<<value;
}

void MontyController::takePicture()
{
    // send picture back to client
    const char* name = "picture.png";
    server->sendPicture(QString::fromLatin1(name));
}

void MontyController::rotateLeft()
{
    server->testMethode();
}

void MontyController::rotateRight()
{

}

void MontyController::zoomIn()
{

}

void MontyController::zoomOut()
{

}

void MontyController::armUp()
{

}

void MontyController::armDown()
{

}
