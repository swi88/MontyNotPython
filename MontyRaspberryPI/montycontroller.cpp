#include "montycontroller.h"
#include "movementstates.h"
#include <QDebug>
#include <opencv2/highgui/highgui.hpp>

MontyController::MontyController(){
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
	Mat* picture;
	emit grab(picture);
	this->savePicture(*picture);
}

void MontyController::savePicture(Mat picture)
{
	imwrite("picture.png", picture);
    // send picture back to client
    const char* name = "picture.png";
    server->sendPicture(QString::fromLatin1(name));
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
