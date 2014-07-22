#ifndef SERVO_H
#define SERVO_H
#include <iostream>
#include <sstream>
#include <string> 
#include <cstring> 
#include <fstream>
#include <unistd.h>
#include <QDebug>
using namespace std;

class Servo
{
public:
        Servo(int pin);
	~Servo();
	void setValue(int value);
    int getCurrentAngle();
    void wait();
    void setAngle(int degree);
private:
	int pin;
    int angle;
	int value;
	FILE *fp;
    int calcAngle(int pwm);
    int calcPWM(int angle);
    void changeAngle(int degree);

};
#endif
