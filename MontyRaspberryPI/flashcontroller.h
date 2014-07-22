#ifndef FLASHCONTROLLER_H
#define FLASHCONTROLLER_H
#include <QThread>
#include <opencv2/video/background_segm.hpp>
#include <QObject>
#include <QTime>
#include "hardware/util/ledstate.h"
#include <opencv/cv.hpp>

using namespace cv;

class FlashController : public QObject
{
    Q_OBJECT
public:
    FlashController();

signals:
    void setFlash(MouthState state);

public slots:
	void checkImage(Mat picture);

private:

    const static int TIME_TO_WAIT;
    QThread* thread;
    Mat picture;
    qint64 lastCheck;
    QTime time;
    int size;
    int histSize;


    double calcHistogramQuantile(double quantile, Mat hist);

private slots:
    void meassureBrigtness();


};

#endif // FLASHCONTROLLER_H
