#include "servo.h"
#include <string>
Servo::Servo(int pin){
	this->pin=pin; 
	fp = fopen("/dev/servoblaster", "w");
 	if (fp == NULL) 
 		printf("Error opening file\n");
	
}
Servo::~Servo(){
	fclose(fp);
}
void Servo::setValue(int value){
	 this->value=value;
     qDebug()<<"set value of servo "<< pin <<" to "<<value;
	 std::ostringstream stm ;
	 stm<<pin<<"="<<value;
	 fprintf(fp,stm.str().c_str());
     fflush(fp);
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
