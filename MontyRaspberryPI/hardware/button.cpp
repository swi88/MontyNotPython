#include "button.h"

#include <QThread>




const int Button::BUTTON= 11; //(GPIO Pin 7)
Button::Button()
{

    pinMode(BUTTON, INPUT);
    pullUpDnControl(BUTTON,PUD_DOWN);
    thread = new QThread();
    this->moveToThread(thread);
    connect(thread,SIGNAL(started()),this,SLOT(checkButtonState()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void Button::checkButtonState()
{
    while (1) {
        if(digitalRead(BUTTON)==1){
            emit buttonPressed();
        }
    }
}