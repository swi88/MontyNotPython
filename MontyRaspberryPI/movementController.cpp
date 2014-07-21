/*
 * movementController.cpp
 *
 *  Created on: 18.07.2014
 *      Author: Dennis
 */

#include "movementController.h"
#include "movementstates.h"

MovementController::MovementController()
{
	wiringPiSetup();
	soll = HOLD_POSITION;
	ist = FLEXIBLE_POSITION;
	ultrasonic = new Ultrasonic();
	stepperRotate = new Stepper(2,3,4,11);
	stepperZoom = new Stepper(17,10,27,22);
	servo = new Servo(2);
    thread = new QThread();
    this->moveToThread(thread);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    servoAngle = servo->getCurrentAngle();
}

void MovementController::performMovement(int movementState) {
	int distance;
	soll = movementState;
	servoAngle = servo->getCurrentAngle();
	// ist-Wert aktualisieren
	if(servoAngle >= 45) ist |= LOWER_END_REACHED;
	else if(servoAngle <= -45) ist |= UPPER_END_REACHED;
	else {
		if((ist & LOWER_END_REACHED) != 0) ist -= LOWER_END_REACHED;
		if((ist & UPPER_END_REACHED) != 0) ist -= UPPER_END_REACHED;
	}
	// falls Grenzwert für die Bewegung bereits erreicht..
	if((soll & ist) > 0) {
		if(soll == MOVE_UP && (ist & ZOOM_OUT_POSITION) == 0 ) soll = ZOOM_OUT;
		else if(soll == MOVE_DOWN && (ist & ZOOM_IN_POSITION) == 0) soll = ZOOM_IN;
		else soll = HOLD_POSITION;
	}
	// Falls Hindernis vor dem Ultraschallsensor erkannt..
	if(soll == HOLD_POSITION) {
		distance = ultrasonic->getDistance();
		if((ist & ZOOM_OUT_POSITION) == 0 && distance < 15) soll = ZOOM_OUT;
	}
	switch(soll) {
	case MOVE_UP: moveUp();
	break;
	case MOVE_DOWN: moveDown();
	break;
	case MOVE_LEFT: moveLeft();
	break;
	case MOVE_RIGHT: moveRight();
	break;
	case ZOOM_IN: zoomIn();
	break;
	case ZOOM_OUT: zoomOut();
	break;
	}
}

void MovementController::moveUp()
{
	servo->setValue(servoAngle - 2);
	servo->setValue(servoAngle - 1);
	servoAngle = servo->getCurrentAngle();
}

void MovementController::moveDown()
{
	servo->setValue(servoAngle + 2);
	servo->setValue(servoAngle + 1);
	servoAngle = servo->getCurrentAngle();
}

void MovementController::moveLeft()
{
	if(!stepperRotate->isActive()) stepperRotate->counterclockwise(5);
}

void MovementController::moveRight()
{
	if(!stepperRotate->isActive()) stepperRotate->clockwise(5);
}

void MovementController::zoomIn()
{
	if(!stepperZoom->isActive()) stepperZoom->clockwise(5);
}

void MovementController::zoomOut()
{
	if(!stepperZoom->isActive()) stepperZoom->counterclockwise(5);
}
