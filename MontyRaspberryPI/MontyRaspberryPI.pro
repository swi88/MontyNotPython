#-------------------------------------------------
#
# Project created by QtCreator 2014-07-02T14:34:59
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = MontyRaspberryPI
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    hardware/util/gpio.cpp \
    hardware/servo.cpp \
    hardware/stepper.cpp

HEADERS += \
    hardware/util/gpio.h \
    hardware/servo.h \
    hardware/stepper.h
