#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <QObject>
#include <wiringPi.h>
#include <sr595.h>
#include <wiringShift.h>
#include "montycontroller.h"

class LEDController : public QObject
{
    Q_OBJECT
public:
    explicit LEDController(int dataPin, int clockPin, int latchPin, int outputEnable);

    void setStartAnimation();
    void setMouthLEDState(MontyController::MounthState state);
    void setInfoLEDState(MontyController::InfoState state);

signals:

public slots:


private:
    int pinOutputEnable;
    int pinLatch;
    int pinClock;
    int pinData;
    int pinBase;
    int pinNumbers;


    int codeInfoCurrent;
    int codeMouthCurrent;

    void writeToRegisters(int value);
};

#endif // LEDCONTROLLER_H
