#include "viewerwindow.h"
#include <QApplication>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <fstream>
#include <stdio.h>

int main(int argc, char *argv[])
{
    pid_t pid;

    // Nos clonamos a nosotros mismos creando un proceso hijo
    pid = fork();

    // Si pid es < 0, fork() falló
    if (pid < 0) {
        // Mostrar la descripción del error y terminar
        std::cerr << strerror(errno) << std::endl;
        exit(10);
    }

    // Si pid es > 0, estamos en el proceso padre
    if (pid > 0) {
        // Terminar el proceso
        exit(0);
    }

    // Si la ejecución llega a este punto, estamos en el hijo

    // Abrir una conexión al demonio syslog
    openlog(argv[0], LOG_NOWAIT | LOG_PID, LOG_USER);

    // Intentar crear una nueva sesión
    if (setsid() < 0) {
        syslog(LOG_ERR, "No fue posible crear una nueva sesión\n");
        exit(11);
    }

    // Cambiar directorio de trabajo
    if ((chdir("/")) < 0) {
        syslog(LOG_ERR, "No fue posible cambiar el directorio de "
               "trabajo a /\n");
        exit(12);
    }

    // Cerrar los descriptores de la E/S estándar
    close(STDIN_FILENO);            // fd 0
    close(STDOUT_FILENO);           // fd 1
    close(STDERR_FILENO);           // fd 2

    // Abrir nuevos descriptores de E/S
    int fd0 = open("/dev/null", O_RDONLY);  // fd0 == 0
    int fd1 = open("/dev/null", O_WRONLY);  // fd0 == 1
    int fd2 = open("/dev/null", O_WRONLY);  // fd0 == 2

    // Cambiar umask
    umask(0);

    syslog(LOG_NOTICE, "Demonio iniciado con éxito\n");

    // archivo con PID
    std::fstream out((QString("/var/run/") + "daemon" + ".pid").toStdString().c_str(),std::fstream::out);
    out << getpid();
    out.close();


    // Inicio del programa
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Grupo01");
    QCoreApplication::setApplicationName("Visor");

    ViewerWindow w;
    setupUnixSignalHandlers();

    int retur = a.exec();

    closelog();
    return retur;
}
