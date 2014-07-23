#include "montycontroller.h"
#include <stddef.h>

MontyController::MontyController()
{
    qRegisterMetaType< cv::Mat >("cv::Mat");
    qRegisterMetaType< Mat >("Mat");
    qRegisterMetaType< MouthState >("MouthState");
    qRegisterMetaType< InfoState >("InfoState");
    server = new Server();
    connect(server,SIGNAL(rotateLeft()),this,SLOT(rotateLeft()));
    connect(server,SIGNAL(rotateRight()),this,SLOT(rotateRight()));
    connect(server,SIGNAL(zoomIn()),this,SLOT(zoomIn()));
    connect(server,SIGNAL(zoomOut()),this,SLOT(zoomOut()));
    connect(server,SIGNAL(armUp()),this,SLOT(armUp()));
    connect(server,SIGNAL(armDown()),this,SLOT(armDown()));
    connect(server,SIGNAL(automaticOff()),this,SLOT(stopAutoControl()));
    connect(server,SIGNAL(automaticOn()),this,SLOT(autoControl()));
    server->listen();

    movementController = new MovementController();
    camera = new Camera(movementController);

    connect(server,SIGNAL(takePicture()),camera,SLOT(takePicture()));
    connect(this, SIGNAL(startAutomatic()), camera, SLOT(startAutomatic()));
    connect(this, SIGNAL(stopAutomatic()), camera, SLOT(stopAutomatic()));
    connect(this, SIGNAL(move(int)), movementController, SLOT(performMovement(int)));
    //LED Controller at Pin 23,24,25 (wiring pi pins 4,5,6)
    ledController = new LEDController(4,5,6);
    ledController->setMouthLEDState(OFF);
    ledController->setInfoLEDState(CONTROLL_AUTO);

    //start flash controller
    flashController = new FlashController();
    connect(flashController,SIGNAL(setFlash(MouthState)),ledController,SLOT(setMouthLEDState(MouthState)));
    connect(camera,SIGNAL(update(Mat)),flashController,SLOT(checkImage(Mat)));
    connect(camera,SIGNAL(setInfoLEDState(InfoState)),ledController,SLOT(setInfoLEDState(InfoState)));
    //startup state
    this->autoControl();
    //this->stopAutoControl();
    qDebug()<<"started";
}


/**
 * @brief automatic mode
 */
void MontyController::autoControl()
{

    qDebug()<<"server automatic controll on";
	this->infoState = CONTROLL_AUTO;
    ledController->setInfoLEDState(CONTROLL_AUTO);
    emit startAutomatic();
}
/**
 * @brief manuel mode
 */
void MontyController::stopAutoControl()
{
    qDebug()<<"server automatic controll off";
	this->infoState = CONTROLL_MANUAL;
    ledController->setInfoLEDState(CONTROLL_MANUAL);
    camera->stopAutomatic();
}

void MontyController::receiveUltrasonicDistance(double value)
{
    qDebug()<<value;
}

void MontyController::rotateLeft()
{
    if (this->infoState == CONTROLL_MANUAL)
        emit move(MOVE_LEFT);
}

void MontyController::rotateRight()
{
    if (this->infoState == CONTROLL_MANUAL)
        emit move(MOVE_RIGHT);
}

void MontyController::zoomIn()
{
    if (this->infoState == CONTROLL_MANUAL)
        emit move(ZOOM_IN);
}

void MontyController::zoomOut()
{
    if (this->infoState == CONTROLL_MANUAL)
        emit move(ZOOM_OUT);
}

void MontyController::armUp()
{
    if (this->infoState == CONTROLL_MANUAL)
        emit move(MOVE_UP);
}

void MontyController::armDown()
{
    if (this->infoState == CONTROLL_MANUAL)
        emit move(MOVE_DOWN);
}
