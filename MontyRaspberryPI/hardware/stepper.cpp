///g++ -Wall -o stepper stepper.cpp


// GPIO PINS 17,18,21,22
#include "stepper.h"
#include "util/gpio.h"
#include <QThread>
#include <unistd.h>
#include <QDebug>
const int Stepper::SEQUENCES[][4]= {
    {1,0,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,1,1},
    {0,0,0,1},
    {1,0,0,1}
};

/**
 * @brief create a new stepper instance
 * @details init the pins
 */
Stepper::Stepper(){
    thread = new QThread();
    this->moveToThread(thread);
    connect(this, SIGNAL(finished()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), this, SLOT(switchActiveState()));
    connect(thread, SIGNAL(started()), this, SLOT(switchActiveState()));
    counter=0;
    active=false;
    GPIO* pin17 = new GPIO("17");
    pin17->export_gpio();
    pin17->setdir_gpio("out");
    gpios.push_back(pin17);

    GPIO* pin10 = new GPIO("10");
    pin10->export_gpio();
    pin10->setdir_gpio("out");
    gpios.push_back(pin10);

    GPIO* pin21 = new GPIO("27");
    pin21->export_gpio();
    pin21->setdir_gpio("out");
    gpios.push_back(pin21);

    GPIO* pin22 = new GPIO("22");
    pin22->export_gpio();
    pin22->setdir_gpio("out");
    gpios.push_back(pin22);
}
/**
 * @brief step clockwise
 * @param steps 100 steps == 10 degree?
 */
void Stepper::clockwise(int steps){
    if(thread->isRunning()){
        thread->terminate();
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
        thread->terminate();
    }
    this->steps = steps;
    connect(thread,SIGNAL(started()),this,SLOT(counterclockwise()));
    thread->start();
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
/**
 * @brief clockwise, called by thread
 */
void Stepper::clockwise()
{
    for(int counter=0; counter<steps; counter++){
        for(int seq=0; seq < 8; seq++){
            for(int i=0;i < 4;i++){
                if(SEQUENCES[seq][i]==1){
                    gpios.at(i)->setval_gpio("1");
                }else gpios.at(i)->setval_gpio("0");
            }
            usleep(5000);
        }

        //cout << counter<<"\n";
    }
    disconnect(thread,SIGNAL(started()),this,SLOT(clockwise()));
     qDebug()<<"end cw";
    emit finished();
}
/**
 * @brief called by thread
 */
void Stepper::counterclockwise()
{
    for(int counter=0; counter<steps; counter++){
        //move to sequence
        for(int seq=7; seq >= 0; seq--){
            for(int i=0;i < 4;i++){
                if(SEQUENCES[seq][i]==1){
                    gpios.at(i)->setval_gpio("1");
                }else gpios.at(i)->setval_gpio("0");
            }
            usleep(5000);
        }

        //cout << counter<<"\n";
    }
    disconnect(thread,SIGNAL(started()),this,SLOT(counterclockwise()));
    qDebug()<<"end ccw";
    emit finished();

}

void Stepper::switchActiveState()
{
    this->active = !active;
}
