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
	soll = HOLD_POSITION;
	ist = FLEXIBLE_POSITION;
    thread = new QThread();
    this->moveToThread(thread);
    connect(thread,SIGNAL(started()),this,SLOT(processVideo()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void MovementController::performMovement(int movementState) {
	soll = movementState;
	// ggf. eine Soll-Korrektur
	if((soll & ist) > 0) {
		// TODO Ultraschallsensor auslesen und in Soll-Korrektur einfließen lassen
		if(soll == MOVE_UP && (ist & ZOOM_OUT_POSITION) == 0 ) soll = ZOOM_OUT;
		else if(soll == MOVE_DOWN && (ist & ZOOM_IN_POSITION) == 0) soll = ZOOM_IN;
		else soll = HOLD_POSITION;
	}
	// TODO Aktorik-Befehle und Ist-Wert-Festlegung (auch emit move(movementState))
}
