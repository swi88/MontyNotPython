#ifndef BUTTON_H
#define BUTTON_H
#include <QThread>
#include <wiringPi.h>

class Button: public QObject
{
    Q_OBJECT
public:
    Button();
    bool isPressed();

private:
    QThread* thread;
    const static int BUTTON;
    bool pressed;

private slots:
    void checkButtonState();
};

#endif // BUTTON_H
