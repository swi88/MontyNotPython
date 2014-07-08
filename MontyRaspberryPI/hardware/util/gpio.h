#ifndef GPIO__H
#define GPIO__H
 
#include <string>
#include<stdio.h>
#include <stdlib.h>
#include <cstring>
using namespace std;
class GPIO
{
public:
    GPIO(int pin);
    int export_gpio(); // exports GPIO
    int unexport_gpio(); // unexport GPIO
    int setdir_gpio(string dir); // Set GPIO Direction
    int setval_gpio(int val); // Set GPIO Value (putput pins)
    string getval_gpio(); // Get GPIO Value (input/ output pins)
    int get_gpionum(); // return the GPIO number associated with the instance of an object
private:
    int gpionum; // GPIO number
    FILE *fd;
     char fName [128];
     char buf [16] ;
};
 
#endif
