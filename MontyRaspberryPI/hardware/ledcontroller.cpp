#include "ledcontroller.h"

LEDController::LEDController(int dataPin,int clockPin,int latchPin, int outputEnable) :QObject()
{
    this->pinClock = clockPin;
    this->pinData = dataPin;
    this->pinLatch = latchPin;
    this->pinOutputEnable = outputEnable;
    this->pinBase = 100;
    this->pinNumbers = 8;
    sr595Setup(pinBase,pinNumbers,dataPin,clockPin,latchPin);
}

void LEDController::setInfoLEDState(MontyController::InfoState state)
{

}

void LEDController::setMouthLEDState(MontyController::MounthState state)
{
    switch (state) {
    case MontyController::FLASH_ON:{
        this->codeMouthCurrent = 192;
        break;
        }

    case MontyController::OFF:{
        this->codeMouthCurrent = 0;
        break;
        }
    }
    writeToRegisters(codeMouthCurrent & codeInfoCurrent);
}

void LEDController::writeToRegisters(int value)
{
    for (int bit = 0 ; bit < 10 ; ++bit)
            digitalWrite (pinBase + bit, value& (1 << bit)) ;
    //set values to output
    digitalWrite(pinOutputEnable,HIGH);
    delay(10);
    digitalWrite(pinOutputEnable,LOW);
}


