#include "ultrasonic.h"
#include <QTime>
#include <QDebug>



const int Ultrasonic::ECHO= 14;
const int Ultrasonic::TRIG= 15;
Ultrasonic::Ultrasonic()
{
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    digitalWrite(TRIG, LOW);
    delay(30);
    QThread* thread = new QThread();
    this->moveToThread(thread);
    connect(thread,SIGNAL(started()),this,SLOT(startMeassurement()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

double Ultrasonic::getDistance()
{
    return distance;
}

void Ultrasonic::startMeassurement()
{
    while(1){
        this->distance = meassureDistance();
        delay(100);
        emit receiveDistance(distance);
    }
}
double Ultrasonic::meassureDistance()
{
    //Send trig pulse
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG, LOW);
    //Wait for echo
    while(digitalRead(ECHO) == LOW);
    //Wait for echo end
    long startTime = micros();
    while(digitalRead(ECHO) == HIGH);
    long travelTime = micros() - startTime;
    //Get distance in cm
    int distance = travelTime / 58;
    return distance;
}
