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

using namespace cv;

class Camera : public QObject
{
	Q_OBJECT
public:
	Camera();
    ~Camera();
	signals:
	void update(Mat picture);
private:
	QThread* thread;
    raspicam::RaspiCam_Cv capture;
    //VideoCapture capture;
	bool end;
	Mat frame;
private slots:
	void startAutomatic(void);
	void stopAutomatic(void);
	void grab(Mat* picture);
};

#endif /* CAMERA_H_ */
