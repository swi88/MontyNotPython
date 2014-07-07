#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "gpio.h"
#include <QDebug>
 
using namespace std;
 
 
GPIO::GPIO(string gnum)
{
    this->gpionum = gnum;
}
 
int GPIO::export_gpio()
{
    qDebug()<<QString::fromStdString(gpionum);
    string export_str = "/sys/class/gpio/export";
    ofstream exportgpio(export_str.c_str()); // Open "export" file. Convert C++ string to C string. Required for all Linux pathnames
    if (exportgpio.fail()){
        qDebug()<< " OPERATION FAILED: Unable to export GPIO"<<QString::fromStdString(gpionum);
        return -1;
    }
 
    exportgpio << this->gpionum ; //write GPIO number to export
    exportgpio << endl;
    exportgpio.close(); //close export file
    return 0;
}
 
int GPIO::unexport_gpio()
{
    string unexport_str = "/sys/class/gpio/unexport";
    ofstream unexportgpio(unexport_str.c_str()); //Open unexport file
    if (unexportgpio < 0){
        cout << " OPERATION FAILED: Unable to unexport GPIO"<< this->gpionum <<" ."<< endl;
        return -1;
    }
 
    unexportgpio << this->gpionum ; //write GPIO number to unexport
    unexportgpio << endl;
    unexportgpio.close(); //close unexport file
    return 0;
}
 
int GPIO::setdir_gpio(string dir)
{
 
    string setdir_str ="/sys/class/gpio/gpio" + this->gpionum + "/direction";
    ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
        if (setdirgpio < 0){
            cout << " OPERATION FAILED: Unable to set direction of GPIO"<< this->gpionum <<" ."<< endl;
            return -1;
        }
 
        setdirgpio << dir; //write direction to direction file
        setdirgpio << endl;
        setdirgpio.close(); // close direction file
        return 0;
}
 
int GPIO::setval_gpio(string val)
{
 
    string setval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
        if (setvalgpio < 0){
            cout << " OPERATION FAILED: Unable to set the value of GPIO"<< this->gpionum <<" ."<< endl;
            return -1;
        }
 
        setvalgpio << val ;//write value to value file
        setvalgpio << endl;
        setvalgpio.close();// close value file 
        return 0;
}
 
int GPIO::getval_gpio(string& val){
 
    string getval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    ifstream getvalgpio(getval_str.c_str());// open value file for gpio
    if (getvalgpio < 0){
        cout << " OPERATION FAILED: Unable to get value of GPIO"<< this->gpionum <<" ."<< endl;
        return -1;
            }
 
    getvalgpio >> val ;  //read gpio value
 
    if(val != "0")
        val = "1";
    else
        val = "0";
 
    getvalgpio.close(); //close the value file
    return 0;
}

string GPIO::get_gpionum(){
 
return this->gpionum;
 
}
