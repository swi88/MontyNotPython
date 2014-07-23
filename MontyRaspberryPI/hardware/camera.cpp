/*
 * camera.cpp
 *
 *  Created on: 18.07.2014
 *      Author: Dennis
 */

#include "camera.h"

using namespace std;

Camera::Camera(MovementController* controller)
{
    //capture(0);
    automaticControl = new AutomaticControl(controller);
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
    connect(automaticControl, SIGNAL(move(int)), controller, SLOT(performMovement(int)));
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
    forever{
        lock.lockForRead();
        if(end){
            lock.unlock();
            break;
        }
        lock.unlock();

    }
    while(!end)
    {
        frame = grab();
        this->automaticControl->update(frame);
	}
}

void Camera::stopAutomatic()
{
    qDebug()<<"stopping automatic controll....";
    lock.lockForWrite();
    end = true;
    lock.unlock();
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
    emit update(frame);
    qDebug()<<"frame grabbed!";
    return frame;
    /**
	if (!capture.read(frame)) {
		cerr << "Unable to read next frame." << endl;
	} else picture = &frame;
    **/
}

void Camera::takePicture()
{
    qDebug()<<"takePicture: start";
    Mat picture = grab();
    if(picture.cols > 1 && picture.rows > 1)
    {
        qDebug()<<"takePicture: before savePicture";
        this->savePicture(picture);
        qDebug()<<"takePicture: end";
    }
    else qDebug()<<"can not save the picture";
}

void Camera::savePicture(Mat picture)
{

    emit setInfoLEDState(TAKE_PHOTO);
    // get current timestamp
    time_t rawtime;
    struct tm * timeinfo;
    char nameBuffer [80];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (nameBuffer,80,"picture_%F_%T.png",timeinfo);

    qDebug()<<"save picture..";
    if (!picture.data)
    {
        qDebug()<<"No image data...";
    }
    if(imwrite(nameBuffer, picture))
    {
        imwrite("last.png", picture);
        qDebug()<<"picture saved!";

        // send picture back to client
        //if(server->sendPicture(QString::fromLatin1(nameBuffer)))
        //	qDebug()<<"picture sent";
        //else
        //	qDebug()<<"can not send the picture!";
    }
    else
    {
        qDebug()<<"cancel: can not save the picture!";
    }
}
