#-------------------------------------------------
#
# Project created by QtCreator 2014-01-28T12:39:34
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia multimediawidgets
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = visor_server
TEMPLATE = app

SOURCES += main.cpp\
        viewerwindow.cpp \
    capturebuffer.cpp \
    cvmatandqimage.cpp \
    c_asincrono.cpp

HEADERS  += viewerwindow.h \
    capturebuffer.h \
    cvmatandqimage.h \
    c_asincrono.h

FORMS    +=

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_video

OTHER_FILES += \
    opencv.pri \
    QtOpenCV.pri

macx {          # Esta configuración específica de Linux y UNIX
    # Variables
    #
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }

    MOC_DIR= ./moc
    OBJECTS_DIR= ./object

    BINDIR  = $$PREFIX/bin
    # DATADIR = $$PREFIX/share
    CONFDIR = /etc/xdg

    isEmpty(VARDIR) {
        VARDIR  = /var/lib/$${TARGET}
    }

    DEFINES += APP_DATADIR=\\\"$$DATADIR\\\"
    DEFINES += APP_VARDIR=\\\"$$VARDIR\\\"
    DEFINES += APP_CONFFILE=\\\"$$CONFDIR/$${TARGET}.ini\\\"

    # Install
    #
    INSTALLS += target config # desktop icon32 vardir

    ## Instalar ejecutable
    target.path = $$BINDIR

    ## Instalar archivo de configuración
    config.path = $$CONFDIR
    config.files += $${TARGET}.ini


    ## Instalar acceso directo en el menú del escritorio
    ## desktop.path = $$DATADIR/applications
    ## desktop.files += $${TARGET}.desktop

    ## Instalar icono de aplicación
    ## icon32.path = $$DATADIR/icons/hicolor/32x32/apps
    ## icon32.files += ./data/32x32/$${TARGET}.png

    ## Crear directorio de archivos variables
    vardir.path = $$VARDIR
    vardir.commands = :
}
