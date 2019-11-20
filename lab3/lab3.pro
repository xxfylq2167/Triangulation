#-------------------------------------------------
#
# Project created by QtCreator 2014-03-21T15:59:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab3
TEMPLATE = app

INCLUDEPATH += C:\Qt\Eigen


SOURCES += main.cpp\
        mainwindow.cpp \
    triangle.cpp \
    qpointf3.cpp \
    linesegments.cpp \
    incircle.cpp \
    determinant.cpp \
    computetricenter.cpp \
    computetriarea.cpp \
    computecircumcirclecenter.cpp

HEADERS  += mainwindow.h \
    triangle.h \
    qpointf3.h \
    linesegments.h \
    incircle.h \
    determinant.h \
    computetricenter.h \
    computetriarea.h \
    computecircumcirclecenter.h

FORMS    += mainwindow.ui
