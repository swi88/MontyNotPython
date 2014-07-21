#include "servo.h"
#include <string>
Servo::Servo(int pin){
	this->pin=pin; 
}
Servo::~Servo(){
	fclose(fp);
}
void Servo::setValue(int value){
	 fp = fopen("/dev/servoblaster", "w");
         if (fp == NULL) 
                printf("Error opening file\n");
        qDebug()<<"set value of servo "<< pin <<" to "<<value;
	fprintf (fp, "%d=%d\n",pin,value) ;
	fflush(fp);
	qDebug()<<"wait";
	sleep(1);
	if(value>this->value) fprintf (fp, "%d=%d\n",pin,value-2);
	else fprintf (fp, "%d=%d\n",pin,value+2) ;
	fclose(fp);
	qDebug()<<"ready";
	this->value = value;

}

int Servo::getCurrentAngle()
{
    return angle;
}


int Servo::calcAngle(int pwm)
{
    /**
      * pwm 50 == 0 degree
      * pwm 240 == 180 °
      * */
    return (18.0/19.0)*pwm-(900/19);
}

int Servo::calcPWM(int angle)
{
    return (190.0/180.0)*angle+50;
}

void Servo::changeAngle(int degree)
{
    setValue(degree*(190.0/180.0)*value);
}

void Servo::setAngle(int degree)
{
    setValue(degree*(190.0/180.0));
}
