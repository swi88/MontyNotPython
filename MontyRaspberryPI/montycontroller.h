#ifndef MONTYCONTROLLER_H
#define MONTYCONTROLLER_H

#include <QObject>
#include "hardware/ultrasonic.h"
#include "hardware/stepper.h"
#include "server.h"

class MontyController : public QObject
{
    Q_OBJECT
public:
    explicit MontyController();
    enum InfoState{
        CONTROLL_AUTO,
        CONTROLL_MANUAL,
        TAKE_PHOTO
    };
    enum MounthState{
        FLASH_ON,
        OFF,
        START_ANIMATION
    };

signals:

public slots:


private:
    Ultrasonic* ultrasonic;
    Stepper* stepperRotate;
    Stepper* stepperZoom;
    Server* server;

private slots:
    void receiveUltrasonicDistance(double value);
};

#endif // MONTYCONTROLLER_H
