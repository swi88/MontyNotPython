/*
 * camera.h
 *
 *  Created on: 18.07.2014
 *      Author: Dennis
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <opencv2/highgui/highgui.hpp>
#include <QObject>
#include <opencv/cv.hpp>
#include <QThread>
#include <raspicam/raspicam_cv.h>
#include <QDebug>
#include <automaticControl.h>
#include <movementController.h>
#include "util/ledstate.h"

using namespace cv;

class Camera : public QObject
{
	Q_OBJECT
public:
    Camera(MovementController* controller);
    ~Camera();
    Mat grab();
signals:
    void update(Mat picture);
    void setInfoLEDState(InfoState state);

public slots:
        void stopAutomatic(void);
        void takePicture();
private:
	QThread* thread;
    AutomaticControl* automaticControl;
    raspicam::RaspiCam_Cv capture;
    bool end;
    bool interruped;

    //VideoCapture capture;

    Mat frame;
private slots:

    void savePicture(Mat picture);
	void startAutomatic(void);
};

#endif /* CAMERA_H_ */
