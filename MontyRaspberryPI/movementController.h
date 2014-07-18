/*
 * movementController.h
 *
 *  Created on: 18.07.2014
 *      Author: Dennis
 */

#ifndef MOVEMENTCONTROLLER_H_
#define MOVEMENTCONTROLLER_H_

#include <QObject>

class MovementController : public QObject
{
    Q_OBJECT
public:
    MovementController();
private:
    QThread* thread;
    int soll;
    int ist;
public slots:
    void performMovement(int movementstate);
};


#endif /* MOVEMENTCONTROLLER_H_ */
