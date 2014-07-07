#ifndef STEPPER_H
#define STEPPER_H
#include<iostream>
#include  <vector>
#include <pthread.h>
#include <QObject>
#include "util/gpio.h"
using namespace std;





class Stepper : public QObject
{
    Q_OBJECT
public:
	Stepper();
    void clockwise(int steps);
    void counterclockwise(int steps);
	bool isActive();
    bool join();
    void stop();
private:
	
	const static int SEQUENCES[][4];
    const static int STEPS;
	vector<GPIO*> gpios;
    int counter;
    QThread* thread;
    bool active;
    int steps;

signals:
    void finished();

 private slots:
    void clockwise();
    void counterclockwise();
    void switchActiveState();
};

#endif
