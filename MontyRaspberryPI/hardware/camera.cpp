/*
 * camera.cpp
 *
 *  Created on: 18.07.2014
 *      Author: Dennis
 */

#include "camera.h"

using namespace std;

Camera::Camera()
{
	capture(0);
	end = true;
    thread = new QThread();
    this->moveToThread(thread);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Cannot open the video cam" << endl;
	}
}

Camera::~Camera()
{
	//delete capture object
	capture.release();
}

void Camera::startAutomatic()
{
	end = false;
	while(!end)
	{
		//read the current frame
		if (!capture.read(frame)) {
			cerr << "Unable to read next frame." << endl;
			continue;
		} else emit update(frame);
	}
}

void Camera::stopAutomatic()
{
	end = true;
}

void Camera::grab(Mat* picture)
{
	if (!capture.read(frame)) {
		cerr << "Unable to read next frame." << endl;
		continue;
	} else picture = &frame;
}
