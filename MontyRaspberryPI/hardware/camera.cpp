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
    capture.set( CV_CAP_PROP_FORMAT, CV_8UC3 );
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
        capture.grab();
        capture.retrieve ( frame);
        emit update(frame);
	}
}

void Camera::stopAutomatic()
{
	end = true;
}

void Camera::grab(Mat* picture)
{
	int tries = 0;
	do {
		capture.grab();
		capture.retrieve (frame);
	    picture = &frame;
		if(picture == nullptr)
		{
			qDebug()<<"frame empty, try to grab again..";
			tries++;
			if(tries == 4)
			{
				qDebug()<<"grabbing failed!";
				return;
			}
		}
	} while (picture == nullptr);
	qDebug()<<"frame grabbed!";
    /**
	if (!capture.read(frame)) {
		cerr << "Unable to read next frame." << endl;
	} else picture = &frame;
    **/
}
