#include "button.h"

#include <QThread>




const int Button::ZOOM_BUTTON = 11; //(GPIO Pin 7)
const int Button::ROTATE_BUTTON = 13;
Button::Button(int pin)
{
	pressed = false;
	this->pin = pin;
    pinMode(this->pin, INPUT);
    pullUpDnControl(this->pin,PUD_DOWN);
    thread = new QThread();
    this->moveToThread(thread);
    connect(thread, SIGNAL(started()), this, SLOT(checkButtonState()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void Button::checkButtonState()
{
    while (1) {
        if(!(digitalRead(pin) == pressed)){
            pressed = !pressed;
        }
        wait(100);
    }
}

bool Button::isPressed()
{
	return pressed;
}
