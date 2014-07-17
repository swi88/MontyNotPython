#-------------------------------------------------
#
# Project created by QtCreator 2014-07-02T14:34:59
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = MontyRaspberryPI
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    hardware/util/gpio.cpp \
    hardware/servo.cpp \
    hardware/stepper.cpp \
    server.cpp \
    hardware/ultrasonic.cpp \
    montycontroller.cpp \
    hardware/ledcontroller.cpp

HEADERS += \
    hardware/util/gpio.h \
    hardware/servo.h \
    hardware/stepper.h \
    server.h \
    hardware/ultrasonic.h \
    montycontroller.h \
    hardware/ledcontroller.h


# OpenCV
#INCLUDEPATH += /opt/opencv/2.4.8/include
#LIBS += -L/opt/opencv/2.4.8/lib
#LIBS += -lopencv_core \
#        -lopencv_highgui \
#        -lopencv_imgproc
unix {
        CONFIG += link_pkgconfig
        PKGCONFIG += opencv
}

# wiring pi

INCLUDEPATH    += /usr/local/include
LIBS += -L/usr/local/lib -lwiringPi
win32{
LIBS += -L "C:/opencv/build/x86/mingw/lib/" -llibopencv_core242 -llibopencv_ml242 -llibopencv_highgui242 -llibopencv_imgproc242 -llibopencv_objdetect242
INCLUDEPATH += "C:/opencv/build/include/"
}
