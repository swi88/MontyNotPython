/*
 * automaticControl.h
 *
 *  Created on: 17.07.2014
 *      Author: Dennis
 */

#ifndef AUTOMATICCONTROL_H_
#define AUTOMATICCONTROL_H_

#include <QObject>
#include <opencv/cv.hpp>

using namespace cv;

class AutomaticControl : public QObject
{
	Q_OBJECT
public:
	AutomaticControl();
	signals:
	void move(int movementState);
	void savePicture(Mat picture);
private:
	QThread* thread;

	// Bildobjekte
	Mat frame;
	Mat fgMaskMOG;
	Ptr<BackgroundSubtractor> pMOG;

	// Statusflags
	bool moveDetected;
	bool pictureCaptured;
	bool moving;

	// Buffer für stabileres Bewegungsrechteck
	int fxBuf[10];
	int fyBuf[10];
	int lxBuf[10];
	int lxBufHigh;
	int lyBuf[10];
	int lyBufHigh;
	uchar bufIdx;

	// Eckpunkte des Bewegungsrechtecks
	// Bsp.: fx = first x, ly = last y
	int fx;
	int fy;
	int lx;
	int ly;

	// temporäre Variablen für verschiedene Zwecke genutzt!
	int tmp;
	int tmp2;

	// Bildgröße und davon abhängige Größen
	int xSize;
	int ySize;
	int xSizeFourth;
	int ySizeFourth;
	int xSizeHalf;
	int xSizeThreeFourth;
	int ySizeHalf;
	int ySizeThreeFourth;
	int colsBorder;
	int rowsBorder;
	int fyBufHigh;
	int fxBufHigh;

	// structured element, wird zur Rauschentfernung benötigt
	Mat elem;
private slots:
	void update(Mat picture);
};

#endif /* AUTOMATICCONTROL_H_ */
