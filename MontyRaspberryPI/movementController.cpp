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
	buttonRotate = new Button(Button::ROTATE_BUTTON);
	buttonZoom = new Button(Button::ZOOM_BUTTON);
	wiringPiSetup();
	soll = HOLD_POSITION;
	ist = FLEXIBLE_POSITION;
	ultrasonic = new Ultrasonic();
    //stepper rotate at pin  2,3,4,11 (wiring pi pins 8,9,7,14)
    stepperRotate = new Stepper(8,9,7,14);
    //stepper zoom at Pin 17,10,27,22; (wiring pi pins 0,12,2,3)
    stepperZoom = new Stepper(0,12,2,3);
    servo = new Servo(0);
    thread = new QThread();
    this->moveToThread(thread);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    servoAngle = servo->getCurrentAngle();
}

void MovementController::performMovement(int movementState) {
	soll = movementState;
	servoAngle = servo->getCurrentAngle();
	// ist-Wert aktualisieren
	if(servoAngle <= 45) ist |= LOWER_END_REACHED;
	else if(servoAngle >= 135) ist |= UPPER_END_REACHED;
	else ist &= SERVO_RESET_MASK;
	// falls Grenzwert für die Bewegung bereits erreicht..
	if((ist & UPPER_END_REACHED) != 0 && (soll & MOVE_UP) != 0)
	{
		soll &= SERVO_RESET_MASK;
		if((soll & ZOOM_IN) == 0 && (ist & ZOOM_OUT_POSITION) == 0 ) soll |= ZOOM_OUT;
	}
	else if((ist & LOWER_END_REACHED) != 0 && (soll & MOVE_DOWN) != 0)
	{
		soll &= SERVO_RESET_MASK;
		if((soll & ZOOM_OUT) == 0 && (ist & ZOOM_IN_POSITION) == 0) soll |= ZOOM_IN;
	}
	// Falls Hindernis vor dem Ultraschallsensor erkannt..
    if((ist & ZOOM_OUT_POSITION) == 0 && ultrasonic->getDistance() < 15) {
    	if((soll & ZOOM_IN) != 0) soll &= ZOOM_RESET_MASK;
    	soll |= ZOOM_OUT;
    }
    if((soll & MOVE_UP) != 0) moveUp();
    else if((soll & MOVE_DOWN) != 0) moveDown();
    if((soll & MOVE_LEFT) != 0) moveLeft();
    else if((soll & MOVE_RIGHT) != 0) moveRight();
    if((soll & ZOOM_IN) != 0) zoomIn();
    else if((soll & ZOOM_OUT) != 0) zoomOut();
}

void MovementController::moveUp()
{
    qDebug()<<"moveUp()";
    servo->setAngle(servoAngle + 20);
    servo->setAngle(servoAngle + 15);
	servoAngle = servo->getCurrentAngle();
}

void MovementController::moveDown()
{
    qDebug()<<"moveDown()";
    servo->setAngle(servoAngle - 15);
	servoAngle = servo->getCurrentAngle();
}

void MovementController::moveLeft()
{
    qDebug()<<"moveLeft() is active "<<stepperRotate->isActive();
    if(!stepperRotate->isActive()) {
        if((ist & RIGHT_END_REACHED) != 0) stepperRotate->clockwise(30);
        else stepperRotate->clockwise(10);
    	if(buttonRotate->isPressed()) ist |= LEFT_END_REACHED;
    	else ist &= ROTATION_RESET_MASK;
    }
}

void MovementController::moveRight()
{
    qDebug()<<"moveRight() is active "<<stepperRotate->isActive();
	if(!stepperRotate->isActive()) {
        if((ist & LEFT_END_REACHED) != 0) stepperRotate->counterclockwise(30);
        else stepperRotate->counterclockwise(10);
    	if(buttonRotate->isPressed()) ist |= RIGHT_END_REACHED;
    	else ist &= ROTATION_RESET_MASK;
	}
}

void MovementController::zoomIn()
{
    qDebug()<<"zoomIn() is active: "<< stepperZoom->isActive();
	if(!stepperZoom->isActive()) {
        if((ist & ZOOM_OUT_POSITION) != 0) stepperZoom->counterclockwise(30);
        else stepperZoom->counterclockwise(10);
    	if(buttonZoom->isPressed()) ist |= ZOOM_IN_POSITION;
    	else ist &= ZOOM_RESET_MASK;
	}
}

void MovementController::zoomOut()
{
    qDebug()<<"zoomOut() is active: "<<stepperZoom->isActive();
	if(!stepperZoom->isActive()) {
        if((ist & ZOOM_IN_POSITION) != 0) stepperZoom->clockwise(30);
        else stepperZoom->clockwise(10);
    	if(buttonZoom->isPressed()) ist |= ZOOM_OUT_POSITION;
    	else ist &= ZOOM_RESET_MASK;
	}
}
