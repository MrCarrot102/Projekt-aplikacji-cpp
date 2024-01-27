#-------------------------------------------------
#
# Project created by QtCreator 2023-10-30T13:46:30
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = projekt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        folderfunctions.cpp \
        snakewidget.cpp \
        widgetfunctions.cpp

HEADERS  += mainwindow.h \
    folderfunctions.h \
    snakewidget.h \
    widgetfunctions.h

FORMS    += mainwindow.ui
