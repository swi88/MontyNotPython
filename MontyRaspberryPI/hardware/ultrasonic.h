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
    double distance;

    const static int TRIG;
    const static int ECHO;

private slots:

    void startMeassurement();

};

#endif // ULTRASONIC_H
