///g++ -Wall -o stepper stepper.cpp


// GPIO PINS 17,18,21,22
#include "stepper.h"
#include "util/gpio.h"
#include <QThread>
#include <unistd.h>
#include <QDebug>
const int Stepper::TIME_TO_WAIT = 10;
/**
 * @brief create a new stepper instance
 * @details init the pins
 */
Stepper::Stepper(int pin1, int pin2,int pin3, int pin4){
    counter=0;
    active=false;
    qDebug()<<"init gpio";
    gpios.push_back(pin1);
    gpios.push_back(pin2);
    gpios.push_back(pin3);
    gpios.push_back(pin4);
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);
    pinMode(pin4, OUTPUT);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, LOW);
    thread = new QThread();
    this->moveToThread(thread);
    connect(this, SIGNAL(finished()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), this, SLOT(switchActiveState()));
    connect(thread, SIGNAL(started()), this, SLOT(switchActiveState()));


}
/**
 * @brief step clockwise
 * @param steps 100 steps == 10 degree?
 */
void Stepper::clockwise(int steps){
    if(thread->isRunning()){
        //TODO freezing program
        //thread->terminate();
        qDebug()<<"thread already running";
    }
    //thread->wait();
    this->steps = steps;
    //thread = new QThread();
    this->moveToThread(thread);
    connect(thread,SIGNAL(started()),this,SLOT(clockwise()));
    thread->start();
}

/**
 * @brief steps counterclockwise
 * @param steps 100 steps == 10 degree?
 */
void Stepper::counterclockwise(int steps){
    if(thread->isRunning()){
        qDebug()<<"thread already running";
    }
    this->steps = steps;
    connect(thread,SIGNAL(started()),this,SLOT(counterclockwise()));
    thread->start();
}

bool Stepper::isActive()
{
    return active;
}
/**
 * @brief wait for thread
 * @return thread is terminated
 */
bool Stepper::join()
{
    return thread->wait();
}
/**
 * @brief stopping the stepper
 */
void Stepper::stop()
{
    thread->terminate();
}

void Stepper::sequence1()
{
    digitalWrite(gpios.at(3), HIGH);
    delay(TIME_TO_WAIT);
    digitalWrite(gpios.at(3), LOW);
}

void Stepper::sequence2()
{
    digitalWrite(gpios.at(3), HIGH);
    digitalWrite(gpios.at(2), HIGH);
    delay(TIME_TO_WAIT);
    digitalWrite(gpios.at(3), LOW);
    digitalWrite(gpios.at(2), LOW);

}

void Stepper::sequence3()
{


    digitalWrite(gpios.at(2), HIGH);
    delay(TIME_TO_WAIT);
    digitalWrite(gpios.at(2), LOW);
}

void Stepper::sequence4()
{
    digitalWrite(gpios.at(1), HIGH);
    digitalWrite(gpios.at(2), HIGH);
    delay(TIME_TO_WAIT);
    digitalWrite(gpios.at(1), LOW);
    digitalWrite(gpios.at(2), LOW);
}

void Stepper::sequence5()
{

    digitalWrite(gpios.at(1), HIGH);
    delay(TIME_TO_WAIT);
    digitalWrite(gpios.at(1), LOW);
}

void Stepper::sequence6()
{

    digitalWrite(gpios.at(0), HIGH);
    digitalWrite(gpios.at(1), HIGH);
    delay(TIME_TO_WAIT);
    digitalWrite(gpios.at(0), LOW);
    digitalWrite(gpios.at(1), LOW);
}

void Stepper::sequence7()
{

    digitalWrite(gpios.at(0), HIGH);
    delay(TIME_TO_WAIT);
    digitalWrite(gpios.at(0), LOW);
}

void Stepper::sequence8()
{

    digitalWrite(gpios.at(3), HIGH);
    digitalWrite(gpios.at(0), HIGH);
    delay(TIME_TO_WAIT);
    digitalWrite(gpios.at(3), LOW);
    digitalWrite(gpios.at(0), LOW);
}

void Stepper::switchState()
{
    active=!active;
}
/**
 * @brief clockwise, called by thread
 */
void Stepper::clockwise()
{
    for(int counter=0; counter<steps; counter++){
        sequence1();
        sequence2();
        sequence3();
        sequence4();
        sequence5();
        sequence6();
        sequence7();
        sequence8();
    }
    disconnect(thread,SIGNAL(started()),this,SLOT(clockwise()));
    qDebug()<<"end cw of stepper "<<gpios.at(0);
    emit finished();
}
/**
 * @brief called by thread
 */
void Stepper::counterclockwise()
{
    for(int counter=0; counter<steps; counter++){
       sequence8();
       sequence7();
       sequence6();
       sequence5();
       sequence4();
       sequence3();
       sequence2();
       sequence1();

        //cout << counter<<"\n";
    }
    disconnect(thread,SIGNAL(started()),this,SLOT(counterclockwise()));
    qDebug()<<"end cw of stepper "<<gpios.at(0);
    emit finished();

}

void Stepper::switchActiveState()
{
    this->active = !active;
}
