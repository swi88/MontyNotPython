//opencv
#include <opencv2/video/background_segm.hpp>
// other project files
#include "automaticControl.h"
#include "montycontroller.h"
#include "movementstates.h"

using namespace std;


const int AutomaticControl::LAST_PICTURE = 60;
const int AutomaticControl::LAST_MOVE = 3;
AutomaticControl::AutomaticControl()
{
	pMOG = new BackgroundSubtractorMOG();
    time.start();
    time.addSecs(120);
    moveTime.start();
    moveTime.addSecs(6);
	moveDetected = false;
	pictureCaptured = false;
	moving = false;

	// Buffer für stabileres Bewegungsrechteck
	lxBufHigh = 0;
	lxBuf[0] = 0;
	lyBufHigh = 0;
	lyBuf[0] = 0;
	bufIdx = 0;

	// Eigentliche Initialisierung findet bei Eintreffen des ersten Frames statt
	xSize = -1;
	ySize = -1;
	fyBufHigh = -1;
	fxBufHigh = -1;
	xSizeFourth = -1;
	fy = -1;
	ySizeFourth = -1;
	colsBorder = -1;
	fx = -1;
	tmp2 = -1;
	xSizeHalf = -1;
	ly = -1;
	xSizeThreeFourth = -1;
	ySizeHalf = -1;
	tmp = -1;
	ySizeThreeFourth = -1;
	lx = -1;
	rowsBorder = -1;
	elem = getStructuringElement(MORPH_ELLIPSE, Size(7, 7), Point(3, 3));

    thread = new QThread();
    this->moveToThread(thread);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void AutomaticControl::update(Mat picture)
{
	qDebug()<<"resize picture..";
	resize(picture, frame, Size(640, 480), 8.3, 8.3, INTER_LANCZOS4);
	cvtColor(frame, frame, CV_BGR2GRAY);
	if(xSize == -1)
	{
		qDebug()<<"init buffer..";
		xSize = frame.cols;
		ySize = frame.rows;
		xSizeHalf = xSize / 2;
		ySizeHalf = ySize / 2;
		xSizeFourth = xSizeHalf / 2;
		ySizeFourth = ySizeHalf / 2;
		xSizeThreeFourth = xSizeHalf + xSizeFourth;
		ySizeThreeFourth = ySizeHalf + ySizeFourth;
		fyBufHigh = ySize;
		fxBufHigh = xSize;
		fxBuf[0] = xSize;
		fyBuf[0] = ySize;
        colsBorder = xSize - 6;
        rowsBorder = ySize - 6;

	}
	//update the background model
	qDebug()<<"background subtraction..";
	pMOG->operator()(frame, fgMaskMOG, 0.25);
	//rauschen entfernen
	qDebug()<<"noise reduction..";
	dilate(fgMaskMOG, fgMaskMOG, elem);
	//eckpunkte finden, falls Bewegung vorhanden
	qDebug()<<"movement detection..";
	fx = 0;
	fy = 0;
	lx = 0;
	ly = 0;
	tmp = 5;
	tmp2 = 5;
	moveDetected = true;
	for (fx = 5; fx < colsBorder && fgMaskMOG.row(tmp2).data[fx] < 127; fx++)
		for (tmp2 = 5; tmp2 < rowsBorder && fgMaskMOG.row(tmp2).data[fx] < 127; tmp2++);
	for (fy = 5; fy < tmp2 && fgMaskMOG.row(fy).data[tmp] < 127; fy++)
		for (tmp = fx; tmp < colsBorder && fgMaskMOG.row(fy).data[tmp] < 127; tmp++);
	if (fx >= colsBorder || fy >= rowsBorder)
		moveDetected = false;
	// bei Bewegung: Rechteck mit Hilfe der Eckpunkte einzeichnen
	if (moveDetected) {
		qDebug()<<"movement detected..";
		pictureCaptured = false;
		tmp = 5;
		tmp2 = 5;
		for (lx = colsBorder; lx >= 5 && fgMaskMOG.row(tmp2).data[lx] < 127; lx--)
			for (tmp2 = rowsBorder; tmp2 >= 5 && fgMaskMOG.row(tmp2).data[lx] < 127; tmp2--);
		for (ly = rowsBorder; ly >= tmp2 && fgMaskMOG.row(ly).data[tmp] < 127; ly--)
			for (tmp = lx; tmp >= 5 && fgMaskMOG.row(ly).data[tmp] < 127; tmp--);

		qDebug()<<"update buffer..";
		tmp = fxBuf[bufIdx];
		fxBuf[bufIdx] = fx;
		if(tmp == fxBufHigh) {
			fxBufHigh = xSize;
			for(tmp2 = 0; tmp2 < 8; tmp2++)
				if(fxBuf[tmp2] < fxBufHigh)
					fxBufHigh = fxBuf[tmp2];
		}
		tmp = lxBuf[bufIdx];
		lxBuf[bufIdx] = lx;
		if(tmp == lxBufHigh) {
			lxBufHigh = 0;
			for(tmp2 = 0; tmp2 < 8; tmp2++)
				if(lxBuf[tmp2] > lxBufHigh)
					lxBufHigh = lxBuf[tmp2];
		}
		tmp = fyBuf[bufIdx];
		fyBuf[bufIdx] = fy;
		if(tmp == fyBufHigh) {
			fyBufHigh = ySize;
			for(tmp2 = 0; tmp2 < 8; tmp2++)
				if(fyBuf[tmp2] < fyBufHigh)
					fyBufHigh = fyBuf[tmp2];
		}
		tmp = lyBuf[bufIdx];
		lyBuf[bufIdx] = ly;
		if(tmp == lyBufHigh) {
			lyBufHigh = 0;
			for(tmp2 = 0; tmp2 < 8; tmp2++)
				if(lyBuf[tmp2] > lyBufHigh)
					lyBufHigh = lyBuf[tmp2];
		}

		bufIdx = bufIdx == 7 ? 0 : bufIdx + 1;
		if(moveTime.elapsed() > LAST_MOVE)
		{
			qDebug()<<"define movement..";
			//define movement
			if(lxBufHigh - fxBufHigh > xSizeThreeFourth) emit move(ZOOM_OUT);
			else if(lyBufHigh > ySizeThreeFourth) emit move(MOVE_DOWN);
			else if(lxBufHigh >= colsBorder - 5) emit move(MOVE_RIGHT);
			else if(fxBufHigh <= 10) emit move(MOVE_LEFT);
			else if(lyBufHigh < ySizeHalf) emit move(MOVE_UP);
			else if(lxBufHigh - fxBufHigh < xSizeHalf) emit move(ZOOM_IN);
			else qDebug()<<"hold position";
			moveTime.restart();
		}
	} else {
		qDebug()<<"no movement detected..";
		// Falls noch kein Foto dieser ruhigen Szene gemacht wurde, mache nun eines.
        if(!pictureCaptured  && time.elapsed() > LAST_PICTURE) {
        	qDebug()<<"new picture command..";
			emit savePicture(picture);
			pictureCaptured = true;
            time.restart();
		}
	}
	qDebug()<<"end automatic controll";
}
