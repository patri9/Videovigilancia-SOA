#-------------------------------------------------
#
# Project created by QtCreator 2014-01-28T12:39:34
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia multimediawidgets
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = visor
TEMPLATE = app

SOURCES += main.cpp\
        viewerwindow.cpp \
    acercadedialog.cpp \
    preferencias.cpp \
    capturebuffer.cpp

HEADERS  += viewerwindow.h \
    acercadedialog.h \
    preferencias.h \
    capturebuffer.h

FORMS    += viewerwindow.ui \
    acercadedialog.ui \
    preferencias.ui
