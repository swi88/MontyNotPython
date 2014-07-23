/*
 * camera.cpp
 *
 *  Created on: 18.07.2014
 *      Author: Dennis
 */

#include "camera.h"

using namespace std;

Camera::Camera(MovementController* movementController)
{
    //capture(0);
    automaticControl = new AutomaticControl(movementController);
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
    connect(automaticControl, SIGNAL(move(int)), movementController, SLOT(performMovement(int)));
    connect(automaticControl, SIGNAL(savePicture(Mat)),this,SLOT(savePicture(Mat)));

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
        	this->automaticControl->update(frame);
        }
        else
            cerr<<"grabbing failed, trying again.."<<endl;
	}
}

void Camera::stopAutomatic()
{
    qDebug()<<"stopping automatic controll....";
    end = true;
}

Mat Camera::grab()
{
    qDebug()<<"grab frame: start";
	int tries = 0;
    while(!(capture.grab())) {
    	qDebug()<<"frame empty, trying to grab again..";
		tries++;
		if(tries == 4)
		{
            cerr<<"grabbing failed!"<<endl;
            return Mat(1,1,CV_8UC3);
		}
	}
    capture.retrieve(frame);
    qDebug()<<"frame grabbed!";
    return frame;
    /**
	if (!capture.read(frame)) {
		cerr << "Unable to read next frame." << endl;
	} else picture = &frame;
    **/
}
