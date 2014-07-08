#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "gpio.h"
#include <errno.h>
 
using namespace std;
 
 
GPIO::GPIO(int pin)
{
    this->gpionum = pin;
}
 
int GPIO::export_gpio()
{
    if ((fd = fopen ("/sys/class/gpio/export", "w")) == NULL)
      {
        fprintf (stderr, "Unable to export GPIO pin %d\n", gpionum) ;
        //exit (1) ;
      }

      fprintf (fd, "%d\n", gpionum) ;
      fclose (fd) ;

    return 0;
}
 
int GPIO::unexport_gpio()
{
    if ((fd = fopen ("/sys/class/gpio/unexport", "w")) == NULL)
      {
        fprintf (stderr, "Unable to unexport GPIO pin %d\n", gpionum) ;
        //exit (1) ;
      }

      fprintf (fd, "%d\n", gpionum) ;
      fclose (fd) ;
      return 0;
}
 
int GPIO::setdir_gpio(string dir)
{
    sprintf (fName, "/sys/class/gpio/gpio%d/direction", gpionum) ;
     if ((fd = fopen (fName, "w")) == NULL)
     {
       fprintf (stderr, "Unable to open GPIO direction interface for pin %d: %s\n", gpionum, strerror (errno)) ;
       //exit (1) ;
     }
     fprintf (fd, "%s\n",dir.c_str()) ;
     fclose(fd);
        return 0;
}
 
int GPIO::setval_gpio(int val)
{
    sprintf (fName, "/sys/class/gpio/gpio%d/value", gpionum) ;
     if ((fd = fopen (fName, "w")) == NULL)
     {
       fprintf (stderr, "Unable to open GPIO value interface for pin %d: %s\n", gpionum, strerror (errno)) ;
       //exit (1) ;
     }
     fprintf (fd, "%d\n",val) ;
     fclose(fd);
     return 0;
}
 
string GPIO::getval_gpio(){
    //TODO is buggy
    int l;
    sprintf (fName, "/sys/class/gpio/gpio%d/value", gpionum) ;
      if ((fd = fopen (fName, "r")) == NULL)
      {
        fprintf (stderr, "No Value file (huh?)\n %d: %s\n", gpionum, strerror (errno)) ;
      }

      if (fread (buf, 16,1,fd) == 0)
        sprintf (buf, "%s", "?") ;

      buf [l] = 0 ;
      if ((buf [strlen (buf) - 1]) == '\n')
        buf [strlen (buf) - 1] = 0 ;
      fclose(fd);
     string str;
     stringstream ss;
     ss << buf;
     ss >> str;
    return str;
}

int GPIO::get_gpionum(){
 
return this->gpionum;
 
}
/**
int  main(int argc, char *argv[])
{
    GPIO* pin17 = new GPIO(17);
    pin17->export_gpio();
    pin17->setdir_gpio("out");
    pin17->setval_gpio(0);

    GPIO* pin10 = new GPIO(10);
    pin10->export_gpio();
    pin10->setdir_gpio("out");
    pin10->setval_gpio(0);

    GPIO* pin21 = new GPIO(27);
    pin21->export_gpio();
    pin21->setdir_gpio("out");
    pin21->setval_gpio(0);

    GPIO* pin22 = new GPIO(22);
    pin22->export_gpio();
    pin22->setdir_gpio("out");
    pin22->setval_gpio(0);
    return 0;
}
**/
