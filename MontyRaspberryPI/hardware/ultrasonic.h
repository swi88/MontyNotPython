#include <QThread>
#include <wiringPi.h>

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

class Ultrasonic : public QObject
{
    Q_OBJECT
public:
    Ultrasonic();
    double getDistance();

signals:
    void receiveDistance(double distance);
private:
    QThread* thread;
    double meassureDistance();
    double median(double*, int length, int index);
    double distance;
    double distances[8];
    int actIndex;

    const static int TRIG;
    const static int ECHO;
    const static int NUMBER_OF_MEASSURES;

private slots:

    void startMeassurement();

};

#endif // ULTRASONIC_H
