/*
 * movementstates.h
 *
 *  Created on: 17.07.2014
 *      Author: Dennis
 */

#ifndef MOVEMENTSTATES_H_
#define MOVEMENTSTATES_H_

// aktueller Soll-Status/Bewegung des Arms
enum
{
    HOLD_POSITION = 0,
    MOVE_UP = 1,
    MOVE_DOWN = 2,
    MOVE_LEFT = 4,
    MOVE_RIGHT = 8,
    ZOOM_IN = 16,
    ZOOM_OUT = 32
};

// aktueller Ist-Status des Arms
enum
{
	FLEXIBLE_POSITION = 0,
	UPPER_END_REACHED = 1,
	LOWER_END_REACHED = 2,
	LEFT_END_REACHED = 4,
	RIGHT_END_REACHED = 8,
	ZOOM_IN_POSITION = 16,
	ZOOM_OUT_POSITION = 32
};


#endif /* MOVEMENTSTATES_H_ */
