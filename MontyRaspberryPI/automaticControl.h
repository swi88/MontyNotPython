/*
 * automaticControl.h
 *
 *  Created on: 17.07.2014
 *      Author: Dennis
 */

#ifndef AUTOMATICCONTROL_H_
#define AUTOMATICCONTROL_H_

class AutomaticControl : public QObject
{
	Q_OBJECT
public:
	AutomaticControl();
	void takePicture(void);
	signals:
	void move(int movementState);
private:
	QThread* thread;
	void performMovement(void);
private slots:
	void processVideo(void);
};


#endif /* AUTOMATICCONTROL_H_ */
