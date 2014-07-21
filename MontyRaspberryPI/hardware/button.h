#ifndef BUTTON_H
#define BUTTON_H
#include <QThread>
#include <wiringPi.h>

class Button: public QObject
{
    Q_OBJECT
public:
    Button(int);
    bool isPressed();
    const static int ZOOM_BUTTON;
    const static int ROTATE_BUTTON;

private:
    QThread* thread;
    bool pressed;
    int pin;

private slots:
    void checkButtonState();
};

#endif // BUTTON_H
