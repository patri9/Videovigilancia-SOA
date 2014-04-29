#include "viewerwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Grupo01");
    QCoreApplication::setApplicationName("Visor");

    ViewerWindow w;
    w.show();

    return a.exec();
}
