#include "flashcontroller.h"
#include <QDebug>

const int FlashController::TIME_TO_WAIT = 200; //200ms
FlashController::FlashController()
{
    //so first picture is using for controll
    time.start();
    time.addSecs(30);
    isRunning = false;

    //init histogram
    this->histSize = 10;

    thread = new QThread();
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    //connect(thread, SIGNAL(started()), this, SLOT(meassureBrigtness()));
    this->moveToThread(thread);
    thread->start();
}

void FlashController::checkImage(Mat picture)
{
    qDebug()<<"check image";
    if(!isRunning && time.elapsed()>TIME_TO_WAIT){
        isRunning = true;
        qDebug()<<"check image 2";
        time.restart();
        this->picture = picture;
        meassureBrigtness();
        isRunning = false;
    }
}

void FlashController::meassureBrigtness()
{

    Mat gray;
    cvtColor(picture,gray,CV_BGR2GRAY);
    /**
    //very simple...
    cv::Scalar cvMean;
    cv::Scalar cvStddev;
    cv::meanStdDev(gray, cvMean, cvStddev);
    if(cvMean.val[0]>200) emit setFlash(OFF);
    else emit setFlash(FLASH_ON);
    **/
    Mat histLuminance;
    float range[] = { 0, 256 };
    const float* histRange = { range };

    calcHist( &gray, 1, 0, Mat(), histLuminance, 1, &histSize, &histRange, true, false);
    this->size= gray.cols*gray.rows;
    double quantil20=calcHistogramQuantile(0.2,histLuminance);
    double quantil80=calcHistogramQuantile(0.6,histLuminance);
    if(quantil80-quantil20>0.4){
        qDebug()<<"exposure okay";
        emit setFlash(OFF);
    }else if(1-quantil80>0.3){
        qDebug()<<"over exposure";
        emit setFlash(FLASH_ON);
    }
    else{
        qDebug()<<"under exposure";
        emit setFlash(OFF);
    }
    //qDebug()<<"flash controller: quantil20:"<<quantil20<<", quantil60"<<quantil80;
}

double FlashController::calcHistogramQuantile(double quantile, Mat hist)
    {
    double result=0;
    for(int bin=0;bin<quantile*histSize;bin++){
        result+=hist.at<float>(bin);
    }
    result/=size;
    return result;
}

