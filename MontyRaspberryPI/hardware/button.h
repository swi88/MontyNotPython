#ifndef BUTTON_H
#define BUTTON_H
#include <QThread>
#include <wiringPi.h>

class Button: public QObject
{
    Q_OBJECT
public:
    Button();

signals:
    void buttonPressed();


private:
    QThread* thread;
    const static int BUTTON;

private slots:
    void checkButtonState();
};

#endif // BUTTON_H
