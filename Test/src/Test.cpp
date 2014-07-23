//opencv
#include <opencv/cv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

// aktueller Soll-Status/Bewegung des Arms
enum
{
    HOLD_POSITION = 0,
    MOVE_UP = 1,
    MOVE_DOWN = 2,
    MOVE_LEFT = 4,
    MOVE_RIGHT = 8,
    ZOOM_IN = 16,
    ZOOM_OUT = 32
};

// aktueller Ist-Status des Arms
enum
{
	FLEXIBLE_POSITION = 0,
	UPPER_END_REACHED = 1,
	LOWER_END_REACHED = 2,
	LEFT_END_REACHED = 4,
	RIGHT_END_REACHED = 8,
	ZOOM_IN_POSITION = 16,
	ZOOM_OUT_POSITION = 32
};

//global variables
int soll = HOLD_POSITION; //soll-Wert für Bewegung
int ist = FLEXIBLE_POSITION; //ist-Wert der Aktorik
Mat frame; //current frame
Mat fgMaskMOG; //fg mask generated by MOG method
Ptr<BackgroundSubtractor> pMOG; //MOG Background subtractor
int keyboard;

//function declarations
void capturePicture(void);
void performMovement(void);
void processVideo(void);

int main(int argc, char* argv[]) {
	//create GUI windows
	namedWindow("Frame");
	namedWindow("FG Mask MOG");
	//create Background Subtractor object
	pMOG = new BackgroundSubtractorMOG();
	//input data coming from a video
	processVideo();
	//destroy GUI windows
	destroyAllWindows();
	return EXIT_SUCCESS;
}

void capturePicture() {
	// TODO Foto schiessen
}

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

	//create the capture object
	VideoCapture capture(0);

	// status flags
	bool moveDetected = false;
	bool pictureCaptured = false;

	// Buffer für stabileres Bewegungsrechteck
	int fxBuf[8];
	int fyBuf[8];
	int lxBuf[8];
	int lxBufHigh = 0;
	lxBuf[0] = 0;
	int lyBuf[8];
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
	Mat elem = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
	Mat reduced;

	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Cannot open the video cam" << endl;
		exit(EXIT_FAILURE);
	}

	// Bildgröße und davon abhängige Größen einmal berechnen und dann speichern (spart Rechenzeit)
	xSize = 640; //get the width of frames of the video
	ySize = 480; //get the height of frames of the video
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
		resize(frame, reduced, Size(640, 480));
		cvtColor(reduced, reduced, CV_BGR2GRAY);
		//update the background model
		pMOG->operator()(reduced, fgMaskMOG, 0.25);
		//rauschen entfernen
		erode(fgMaskMOG, fgMaskMOG, elem);
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

//			rectangle(frame, cv::Point(fxBufHigh, fyBufHigh), cv::Point(lxBufHigh, lyBufHigh),
//					cv::Scalar(255, 0, 0), 5);

			//define movement
			if(xSize - lxBufHigh < fxBufHigh) {
				if(fxBufHigh < xSizeFourth) cout << "move left" << endl;
				else if(fxBufHigh > xSizeHalf) cout << "move right" << endl;
			} else {
				if(lxBufHigh > xSizeThreeFourth) cout << "move left" << endl;
				else if(lxBufHigh < xSizeFourth) cout << "move right" << endl;
			}
			if(lyBufHigh > ySizeThreeFourth) cout << "move down" << endl;
			else if(lyBufHigh < ySizeHalf) cout << "move up" << endl;
			if(lxBufHigh - fxBufHigh < xSizeHalf) cout << "zoom in" << endl;
			else if(lxBufHigh - fxBufHigh > xSizeThreeFourth) cout << "zoom out" << endl;
			performMovement();
		} else {
			// Falls noch kein Foto dieser ruhigen Szene gemacht wurde, mache nun eines.
			if(!pictureCaptured) {
				capturePicture();
				pictureCaptured = true;
			}
		}
		//show the current frame and the fg masks
		if (frame.cols > 0 && frame.rows > 0)
			imshow("Frame", frame);
		if (fgMaskMOG.cols > 0 && fgMaskMOG.rows > 0)
			imshow("FG Mask MOG", fgMaskMOG);
		//get the input from the keyboard
		keyboard = waitKey(30);
	}
	//delete capture object
	capture.release();
}
