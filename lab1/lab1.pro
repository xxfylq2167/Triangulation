#-------------------------------------------------
#
# Project created by QtCreator 2014-01-23T14:31:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    determinant.cpp \
    computetriarea.cpp \
    computetricenter.cpp \
    intersectionsegments.cpp \
    linesegments.cpp \
    sortvertexbycoordinate.cpp

HEADERS  += mainwindow.h \
    determinant.h \
    computetriarea.h \
    computetricenter.h \
    intersectionsegments.h \
    linesegments.h \
    sortvertexbycoordinate.h

FORMS    += mainwindow.ui
