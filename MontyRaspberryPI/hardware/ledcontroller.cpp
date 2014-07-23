#include "ledcontroller.h"

#include <QDebug>

const static int CODE_FLASH_ON = 31;
const static int CODE_FLASH_OFF = 0;

const static int CODE_INFO_LED_GREEN = 32;
const static int CODE_INFO_LED_RED = 64;

const static int CODE_INFO_LED_TAKE_PICTURE = 96;

LEDController::LEDController(int dataPin,int clockPin,int latchPin) :QObject()
{
    this->pinClock = clockPin;
    this->pinData = dataPin;
    this->pinLatch = latchPin;
    this->pinBase = 100;
    this->pinNumbers = 8;
    sr595Setup(pinBase,pinNumbers,dataPin,clockPin,latchPin);
    writeToRegisters(0);
    delay(100);
    //start animation
    startUp();
 }

void LEDController::setInfoLEDState(InfoState state)
{
    switch (state) {
    case CONTROLL_AUTO:{
        controllModeCurrent = state;
        this->codeInfoCurrent = CODE_INFO_LED_GREEN;
        setLEDs();

        break;
    }
    case CONTROLL_MANUAL:{
        controllModeCurrent = state;
        this->codeInfoCurrent = CODE_INFO_LED_RED;
        setLEDs();
        break;
    }
    case TAKE_PHOTO:{
        //blinking
        int oldCode = this->codeInfoCurrent;
        for (int i = 0; i < 5; i++) {
            this->codeInfoCurrent =CODE_INFO_LED_TAKE_PICTURE | codeInfoCurrent;
            setLEDs();
            delay(50);
            this->codeInfoCurrent =oldCode;
            delay(50);
        }
        setInfoLEDState(controllModeCurrent);
        break;
    }

    default:
        break;
    }
}

void LEDController::startUp()
{
    writeToRegisters(1 | CODE_INFO_LED_GREEN);
    delay(100);
    writeToRegisters(3 | CODE_INFO_LED_GREEN);
    delay(100);
    writeToRegisters(7 | CODE_INFO_LED_GREEN);
    delay(100);
    writeToRegisters(15 | CODE_INFO_LED_GREEN);
    delay(100);
    writeToRegisters(31 | CODE_INFO_LED_GREEN);
    delay(100);
    writeToRegisters(CODE_FLASH_OFF | CODE_INFO_LED_GREEN);
    delay(100);
    writeToRegisters(CODE_INFO_LED_GREEN);
}

void LEDController::setMouthLEDState(MouthState state)
{
    switch (state) {
    case FLASH_ON:{
        this->codeMouthCurrent = CODE_FLASH_ON;
        break;
        }

    case OFF:{
        this->codeMouthCurrent = CODE_FLASH_OFF ;
        break;
        }
    }
    setLEDs();
}

void LEDController::writeToRegisters(int value)
{
    for (int bit = 0 ; bit < 8 ; ++bit){
              digitalWrite (100 + bit, value & (1 << bit)) ;
              delay (1) ;
    }

}
void LEDController::setLEDs()
{
    int value = codeMouthCurrent | codeInfoCurrent;
    writeToRegisters(value);
}


