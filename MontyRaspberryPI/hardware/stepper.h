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

    void sequence1();
    void sequence2();
    void sequence3();
    void sequence4();
    void sequence5();
    void sequence6();
    void sequence7();
    void sequence8();

    const static int TIME_TO_WAIT;
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
