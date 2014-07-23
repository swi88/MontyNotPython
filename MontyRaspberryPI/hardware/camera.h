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

using namespace cv;

class Camera : public QObject
{
	Q_OBJECT
public:
	Camera(MovementController*);
    ~Camera();
	Mat grab();
	signals:
	void update(Mat picture);

public slots:
        void stopAutomatic(void);
private:
	QThread* thread;
    AutomaticControl* automaticControl;
    raspicam::RaspiCam_Cv capture;
    //VideoCapture capture;
	bool end;
	Mat frame;
private slots:
	void startAutomatic(void);
};

#endif /* CAMERA_H_ */
