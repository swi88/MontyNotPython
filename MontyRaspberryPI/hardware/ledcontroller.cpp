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
    writeToRegisters(CODE_FLASH_ON);
   delay(100);
   writeToRegisters(CODE_FLASH_OFF);
   writeToRegisters(CODE_INFO_LED_GREEN);

   writeToRegisters(CODE_INFO_LED_TAKE_PICTURE);



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
        setLEDs();

        setInfoLEDState(controllModeCurrent);
        break;
    }

    default:
        break;
    }
}

void LEDController::setMouthLEDState(MounthState state)
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


