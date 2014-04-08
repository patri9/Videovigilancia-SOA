#-------------------------------------------------
#
# Project created by QtCreator 2014-02-01T19:47:29
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia multimediawidgets
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Visor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ayuda.cpp \
    preferencias_camara.cpp \
    vista_capturador.cpp

HEADERS  += mainwindow.h \
    ayuda.h \
    preferencias_camara.h \
    vista_capturador.h

FORMS    += mainwindow.ui \
    ayuda.ui \
    preferencias_camara.ui
