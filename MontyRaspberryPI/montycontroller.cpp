#include "montycontroller.h"
#include <QDebug>

MontyController::MontyController(){
    //init wiring pi
    //wiringPiSetup();
    //ultrasonic = new Ultrasonic();
    //stepperRotate = new Stepper(2,3,4,11);
    //stepperZoom = new Stepper(17,10,27,22);
	ultrasonic = new Ultrasonic();
	stepperRotate = new Stepper(0, 0, 0, 0);
	stepperZoom = new Stepper(1, 1, 1, 1);
	servo = new Servo(2);
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

    camera = new Camera();
    automaticControl = new AutomaticControl();
    movementController = new MovementController();
    connect(camera, SIGNAL(update(Mat*)), automaticControl, SLOT(update(Mat*)));
    connect(this, SIGNAL(startAutomatic()), camera, SLOT(startAutomatic()));
    connect(this, SIGNAL(stopAutomatic()), camera, SLOT(stopAutomatic()));
    connect(this, SIGNAL(grab(Mat*)), camera, SLOT(grab(Mat*)));
    connect(automaticControl, SIGNAL(move(int)), movementController, SLOT(performMovement(int)));
    connect(this, SIGNAL(move(int)), movementController, SLOT(performMovement(performMovement(int))));
}

void MontyController::receiveUltrasonicDistance(double value)
{
    qDebug()<<value;
}

void MontyController::takePicture()
{

}

void MontyController::savePicture(Mat picture)
{
    // send picture back to client
    string name = "picture.png";
    server->sendPicture(name);
}

void MontyController::rotateLeft()
{

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
