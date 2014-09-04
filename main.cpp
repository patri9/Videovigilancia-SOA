#include "viewerwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QCoreApplication

    QCoreApplication::setOrganizationName("Grupo01");
    QCoreApplication::setApplicationName("Visor");

    ViewerWindow w;

    setupUnixSignalHandlers();

    return a.exec();
}
