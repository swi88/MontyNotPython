#include "ultrasonic.h"
#include <QTime>
#include <QDebug>



const int Ultrasonic::ECHO= 14;
const int Ultrasonic::TRIG= 15;

const int Ultrasonic::NUMBER_OF_MEASSURES = 8;

Ultrasonic::Ultrasonic()
{
	actIndex = 0;
	distance = 0.0;
	distances[8];
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    digitalWrite(TRIG, LOW);
    delay(30);
    thread = new QThread();
    this->moveToThread(thread);
    connect(thread, SIGNAL(started()), this, SLOT(startMeassurement()));
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
    	distances[actIndex] = meassureDistance();
    	actIndex = actIndex == 7 ? 0 : actIndex + 1;
        this->distance = median(distances, 8, actIndex);
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

double Ultrasonic::median(double* array, int length, int index)
{
	int i;
	int count1 = 0;
	int count2 = 0;
	double array1[length];
	double array2[length];
	if(length == 1) return array[0];
	for(i = 0; i < length; i++) {
		if(array[i] < array[index])
		{
			array1[count1] = array[i];
			count1++;
		}
		else if(array[i] > array[index])
		{
			array2[count2] = array[i];
			count2++;
		}
	}
	if(count1 == count2) return array[index];
	else if(count1 > count2) return median(array1, count1, count1 / 2);
	else return median(array2, count2, count2 / 2);
}
