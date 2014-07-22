#include "montycontroller.h"
#include <stddef.h>

MontyController::MontyController()
{
	infoState = CONTROLL_MANUAL;
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
    connect(camera, SIGNAL(update(Mat)), automaticControl, SLOT(update(Mat)));
    connect(this, SIGNAL(startAutomatic()), camera, SLOT(startAutomatic()));
    connect(this, SIGNAL(stopAutomatic()), camera, SLOT(stopAutomatic()));
    connect(this, SIGNAL(grab(Mat*)), camera, SLOT(grab(Mat*)));
    connect(automaticControl, SIGNAL(move(int)), movementController, SLOT(performMovement(int)));
    connect(this, SIGNAL(move(int)), movementController, SLOT(performMovement(int)));
    //LED Controller at Pin 23,24,25 (wiring pi pins 4,5,6)
    ledController = new LEDController(4,5,6);
    ledController->setMouthLEDState(OFF);
    ledController->setInfoLEDState(CONTROLL_AUTO);

    //start flash controller
    flashController = new FlashController();
    connect(flashController,SIGNAL(setFlash(MouthState)),ledController,SLOT(setMouthLEDState(MouthState)));
    connect(camera,SIGNAL(update(Mat)),flashController,SLOT(checkImage(Mat)));
    qDebug()<<"started";
}

void MontyController::autoControl()
{
	this->infoState = CONTROLL_AUTO;
	emit startAutomatic();
}

void MontyController::stopAutoControl()
{
	this->infoState = CONTROLL_MANUAL;
	emit stopAutomatic();
}

void MontyController::receiveUltrasonicDistance(double value)
{
    qDebug()<<value;
}

void MontyController::takePicture()
{
	Mat* picture;
	emit grab(picture);
    flashController->checkImage(picture);
	this->savePicture(*picture);
}

void MontyController::savePicture(Mat picture)
{
    // get current timestamp
    time_t rawtime;
    struct tm * timeinfo;
    char nameBuffer [80];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (nameBuffer,80,"picture_%F_%T.png",timeinfo);

    qDebug()<<"save picture..";
    if(imwrite(nameBuffer, picture))
    {
		qDebug()<<"picture saved!";

		// send picture back to client
		if(server->sendPicture(QString::fromLatin1(nameBuffer)))
			qDebug()<<"picture sent";
		else
			qDebug()<<"can not send the picture!";
    }
    else
    {
    	qDebug()<<"cancel: can not save the picture!";
    }
}

void MontyController::rotateLeft()
{
    server->testMethode();
	emit move(MOVE_LEFT);
}

void MontyController::rotateRight()
{
	emit move(MOVE_RIGHT);
}

void MontyController::zoomIn()
{
	emit move(ZOOM_IN);
}

void MontyController::zoomOut()
{
	emit move(ZOOM_OUT);
}

void MontyController::armUp()
{
	emit move(MOVE_UP);
}

void MontyController::armDown()
{
	emit move(MOVE_DOWN);
}
