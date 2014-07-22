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
    //capture(0);
    capture.set( CV_CAP_PROP_FORMAT, CV_8UC3 );
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
    if (!capture.open()) {
        cerr<<"Error opening the camera"<<endl;
    }
	end = true;
    thread = new QThread();
    this->moveToThread(thread);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

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
        if(capture.grab()){
            capture.retrieve(frame);
        	emit update(frame);
        }
        else
            cerr<<"grabbing failed, trying again.."<<endl;
      sleep();
	}
}

void Camera::stopAutomatic()
{
    cerr<<"stop camera"<<endl;
	end = true;
}

void Camera::grab(Mat* picture)
{
	int tries = 0;
    while(!(capture.grab())) {
        capture.retrieve(frame);
        cerr<<"frame empty, trying to grab again..";
		tries++;
		if(tries == 4)
		{
            cerr<<"grabbing failed!"<<endl;
			return;
		}
	}
    picture = &frame;
    cerr<<"frame grabbed!"<<endl;
    /**
	if (!capture.read(frame)) {
		cerr << "Unable to read next frame." << endl;
	} else picture = &frame;
    **/
}
