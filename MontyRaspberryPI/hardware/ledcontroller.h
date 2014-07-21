#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <QObject>
#include <wiringPi.h>
#include <sr595.h>
#include <wiringShift.h>
#include "hardware/util/ledstate.h"

class LEDController : public QObject
{
    Q_OBJECT
public:
    explicit LEDController(int dataPin, int clockPin, int latchPin);

    void setStartAnimation();
    void setMouthLEDState(MouthState state);
    void setInfoLEDState(InfoState state);

signals:

public slots:


private:
    int pinOutputEnable;
    int pinLatch;
    int pinClock;
    int pinData;
    int pinBase;
    int pinNumbers;
    InfoState controllModeCurrent;

    int codeInfoCurrent;
    int codeMouthCurrent;

    void setLEDs();
    void writeToRegisters(int value);
};

#endif // LEDCONTROLLER_H
