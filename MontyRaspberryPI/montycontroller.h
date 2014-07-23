#ifndef MONTYCONTROLLER_H
#define MONTYCONTROLLER_H

#include <QObject>
#include "server.h"
#include "automaticControl.h"
#include "movementController.h"
#include "hardware/camera.h"
#include "hardware/ledcontroller.h"
#include <opencv/cv.hpp>
#include "flashcontroller.h"
#include "movementstates.h"
#include <QDebug>
#include <opencv2/highgui/highgui.hpp>
#include "hardware/util/ledstate.h"
#include <ctime>
using namespace cv;

class MontyController : public QObject
{
    Q_OBJECT
public:
    explicit MontyController();


signals:
	void startAutomatic(void);
	void stopAutomatic(void);
	void move(int movementstate);
    //void grab(Mat* picture);

public slots:


private:
	int infoState;
    Server* server;
    AutomaticControl* automaticControl;
    MovementController* movementController;
    LEDController* ledController;
    FlashController* flashController;
    Camera* camera;

private slots:
    void receiveUltrasonicDistance(double value);
    void takePicture();
    void savePicture(Mat picture);
    void rotateLeft();
    void rotateRight();
    void zoomIn();
    void zoomOut();
    void armUp();
    void armDown();
    void autoControl();
    //void manuelControl();
    void stopAutoControl();
};

#endif // MONTYCONTROLLER_H
