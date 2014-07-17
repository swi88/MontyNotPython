//opencv
#include <opencv/cv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <raspicam/raspicam_cv.h>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
// other project files
#include "montycontroller.h"
#include "movementstates.h"

using namespace cv;
using namespace std;

//global variables
int soll = HOLD_POSITION; //soll-Wert für Bewegung
int ist = FLEXIBLE_POSITION; //ist-Wert der Aktorik
Mat frame; //current frame
Mat fgMaskMOG; //fg mask generated by MOG method
Ptr<BackgroundSubtractor> pMOG; //MOG Background subtractor
int keyboard;

//function declarations
void performMovement(void);
void processVideo(void);

void performMovement() {
	// ggf. eine Soll-Korrektur
	if((soll & ist) > 0) {
		// TODO Ultraschallsensor auslesen und in Soll-Korrektur einfließen lassen
		if(soll == MOVE_UP && (ist & ZOOM_OUT_POSITION) == 0 ) soll = ZOOM_OUT;
		else if(soll == MOVE_DOWN && (ist & ZOOM_IN_POSITION) == 0) soll = ZOOM_IN;
		else soll = HOLD_POSITION;
	}
	// TODO Aktorik-Befehle und Ist-Wert-Festlegung
}

void processVideo() {
	// Hier beginnt die eigentliche Hauptfunktionalität!
	// Initialisierungen:

	//create Background Subtractor object
	pMOG = new BackgroundSubtractorMOG();

	//create the capture object
	raspicam::RaspiCam_Cv capture(0);

	// status flags
	bool moveDetected = false;
	bool pictureCaptured = false;
	bool moving = false;

	// Buffer für stabileres Bewegungsrechteck
	int fxBuf[10];
	int fyBuf[10];
	int lxBuf[10];
	int lxBufHigh = 0;
	lxBuf[0] = 0;
	int lyBuf[10];
	int lyBufHigh = 0;
	lyBuf[0] = 0;
	uchar bufIdx = 0;

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

	// structured element, wird zur Rauschentfernung benötigt
	Mat elem;

	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Cannot open the video cam" << endl;
		exit(EXIT_FAILURE);
	}

	// Bildgröße und davon abhängige Größen einmal berechnen und dann speichern (spart Rechenzeit)
	xSize = capture.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	ySize = capture.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
	xSizeHalf = xSize / 2;
	ySizeHalf = ySize / 2;
	xSizeFourth = xSizeHalf / 2;
	ySizeFourth = ySizeHalf / 2;
	xSizeThreeFourth = xSizeHalf + xSizeFourth;
	ySizeThreeFourth = ySizeHalf + ySizeFourth;

	// fehlende Bufferinitialisierungen in Abhängigkeit der Bildgröße
	int fyBufHigh = ySize;
	int fxBufHigh = xSize;
	fxBuf[0] = xSize;
	fyBuf[0] = ySize;

	// eigentliche main-Schleife
	//read input data. ESC or 'q' for quitting
	while ((char) keyboard != 'q' && (char) keyboard != 27) {
		//read the current frame
		if (!capture.read(frame)) {
			cerr << "Unable to read next frame." << endl;
			continue;
		}
		//update the background model
		pMOG->operator()(frame, fgMaskMOG, 0.25);
		//rauschen entfernen
		elem = getStructuringElement(MORPH_ELLIPSE, Size(7, 7), Point(3, 3));
		dilate(fgMaskMOG, fgMaskMOG, elem);
		//eckpunkte finden, falls Bewegung vorhanden
		fx = 0;
		fy = 0;
		lx = 0;
		ly = 0;
		tmp = 5;
		tmp2 = 5;
		moveDetected = true;
		colsBorder = fgMaskMOG.cols - 6;
		rowsBorder = fgMaskMOG.rows - 6;
		for (fx = 5; fx < colsBorder && fgMaskMOG.row(tmp2).data[fx] < 127; fx++)
			for (tmp2 = 5; tmp2 < rowsBorder && fgMaskMOG.row(tmp2).data[fx] < 127; tmp2++);
		for (fy = 5; fy < tmp2 && fgMaskMOG.row(fy).data[tmp] < 127; fy++)
			for (tmp = fx; tmp < colsBorder && fgMaskMOG.row(fy).data[tmp] < 127; tmp++);
		if (fx >= colsBorder || fy >= rowsBorder)
			moveDetected = false;
		// bei Bewegung: Rechteck mit Hilfe der Eckpunkte einzeichnen
		if (moveDetected) {
			pictureCaptured = false;
			tmp = 5;
			tmp2 = 5;
			for (lx = colsBorder; lx >= 5 && fgMaskMOG.row(tmp2).data[lx] < 127; lx--)
				for (tmp2 = rowsBorder; tmp2 >= 5 && fgMaskMOG.row(tmp2).data[lx] < 127; tmp2--);
			for (ly = rowsBorder; ly >= tmp2 && fgMaskMOG.row(ly).data[tmp] < 127; ly--)
				for (tmp = lx; tmp >= 5 && fgMaskMOG.row(ly).data[tmp] < 127; tmp--);

			tmp = fxBuf[bufIdx];
			fxBuf[bufIdx] = fx;
			if(tmp == fxBufHigh) {
				fxBufHigh = xSize;
				for(tmp2 = 0; tmp2 < 10; tmp2++)
					if(fxBuf[tmp2] < fxBufHigh)
						fxBufHigh = fxBuf[tmp2];
			}
			tmp = lxBuf[bufIdx];
			lxBuf[bufIdx] = lx;
			if(tmp == lxBufHigh) {
				lxBufHigh = 0;
				for(tmp2 = 0; tmp2 < 10; tmp2++)
					if(lxBuf[tmp2] > lxBufHigh)
						lxBufHigh = lxBuf[tmp2];
			}
			tmp = fyBuf[bufIdx];
			fyBuf[bufIdx] = fy;
			if(tmp == fyBufHigh) {
				fyBufHigh = ySize;
				for(tmp2 = 0; tmp2 < 10; tmp2++)
					if(fyBuf[tmp2] < fyBufHigh)
						fyBufHigh = fyBuf[tmp2];
			}
			tmp = lyBuf[bufIdx];
			lyBuf[bufIdx] = ly;
			if(tmp == lyBufHigh) {
				lyBufHigh = 0;
				for(tmp2 = 0; tmp2 < 10; tmp2++)
					if(lyBuf[tmp2] > lyBufHigh)
						lyBufHigh = lyBuf[tmp2];
			}

			bufIdx = bufIdx == 9 ? 0 : bufIdx + 1;

			rectangle(frame, cv::Point(fxBufHigh, fyBufHigh), cv::Point(lxBufHigh, lyBufHigh),
					cv::Scalar(255, 0, 0), 5);

			//define movement
			if(lxBufHigh - fxBufHigh > xSizeThreeFourth) soll = ZOOM_OUT;
			else if(lyBufHigh > ySizeThreeFourth) soll = MOVE_DOWN;
			else if(lxBufHigh >= colsBorder - 5) soll = MOVE_RIGHT;
			else if(fxBufHigh <= 10) soll = MOVE_LEFT;
			else if(lyBufHigh < ySizeHalf) soll = MOVE_UP;
			else if(lxBufHigh - fxBufHigh < xSizeHalf) soll = ZOOM_IN;
			else soll = HOLD_POSITION;

			switch(soll) {
			case 0: cout << "move_up" << endl;
			break;
			case 1: cout << "move_down" << endl;
			break;
			case 2: cout << "move_left" << endl;
			break;
			case 3: cout << "move_right" << endl;
			break;
			case 4: cout << "zoom_in" << endl;
			break;
			case 5: cout << "zoom_out" << endl;
			break;
			default: cout << "hold_position" << endl;
			break;
			}
			performMovement();
		} else {
			// Falls noch kein Foto dieser ruhigen Szene gemacht wurde, mache nun eines.
			if(!pictureCaptured) {
				// TODO MontyController::takePicture();
				pictureCaptured = true;
			}
		}
	}
	//delete capture object
	capture.release();
}
