/********************************************************************************
** Form generated from reading UI file 'viewerwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWERWINDOW_H
#define UI_VIEWERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ViewerWindow
{
public:
    QAction *actionAbrir;
    QAction *actionSalir;
    QAction *actionAcerca_de;
    QAction *actionCapturar;
    QAction *actionPreferencias;
    QAction *actionCaptura_de_red;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QPushButton *stopbutton;
    QSpacerItem *horizontalSpacer;
    QPushButton *Quit;
    QPushButton *pause;
    QPushButton *playbutton;
    QCheckBox *reproducir;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menuArchivo;
    QMenu *menuAcerca_de;
    QMenu *menuEditar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ViewerWindow)
    {
        if (ViewerWindow->objectName().isEmpty())
            ViewerWindow->setObjectName(QStringLiteral("ViewerWindow"));
        ViewerWindow->resize(509, 428);
        actionAbrir = new QAction(ViewerWindow);
        actionAbrir->setObjectName(QStringLiteral("actionAbrir"));
        actionSalir = new QAction(ViewerWindow);
        actionSalir->setObjectName(QStringLiteral("actionSalir"));
        actionAcerca_de = new QAction(ViewerWindow);
        actionAcerca_de->setObjectName(QStringLiteral("actionAcerca_de"));
        actionCapturar = new QAction(ViewerWindow);
        actionCapturar->setObjectName(QStringLiteral("actionCapturar"));
        actionPreferencias = new QAction(ViewerWindow);
        actionPreferencias->setObjectName(QStringLiteral("actionPreferencias"));
        actionCaptura_de_red = new QAction(ViewerWindow);
        actionCaptura_de_red->setObjectName(QStringLiteral("actionCaptura_de_red"));
        centralWidget = new QWidget(ViewerWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        stopbutton = new QPushButton(centralWidget);
        stopbutton->setObjectName(QStringLiteral("stopbutton"));

        gridLayout->addWidget(stopbutton, 3, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 0, 1, 1);

        Quit = new QPushButton(centralWidget);
        Quit->setObjectName(QStringLiteral("Quit"));

        gridLayout->addWidget(Quit, 3, 4, 1, 1);

        pause = new QPushButton(centralWidget);
        pause->setObjectName(QStringLiteral("pause"));

        gridLayout->addWidget(pause, 3, 2, 1, 1);

        playbutton = new QPushButton(centralWidget);
        playbutton->setObjectName(QStringLiteral("playbutton"));

        gridLayout->addWidget(playbutton, 3, 1, 1, 1);

        reproducir = new QCheckBox(centralWidget);
        reproducir->setObjectName(QStringLiteral("reproducir"));

        gridLayout->addWidget(reproducir, 3, 5, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setScaledContents(true);

        gridLayout->addWidget(label, 0, 0, 1, 6);

        ViewerWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ViewerWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 509, 22));
        menuArchivo = new QMenu(menuBar);
        menuArchivo->setObjectName(QStringLiteral("menuArchivo"));
        menuAcerca_de = new QMenu(menuBar);
        menuAcerca_de->setObjectName(QStringLiteral("menuAcerca_de"));
        menuEditar = new QMenu(menuBar);
        menuEditar->setObjectName(QStringLiteral("menuEditar"));
        ViewerWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ViewerWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ViewerWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ViewerWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ViewerWindow->setStatusBar(statusBar);

        menuBar->addAction(menuArchivo->menuAction());
        menuBar->addAction(menuEditar->menuAction());
        menuBar->addAction(menuAcerca_de->menuAction());
        menuArchivo->addAction(actionAbrir);
        menuArchivo->addAction(actionCapturar);
        menuArchivo->addAction(actionCaptura_de_red);
        menuArchivo->addSeparator();
        menuArchivo->addAction(actionSalir);
        menuAcerca_de->addAction(actionAcerca_de);
        menuEditar->addAction(actionPreferencias);

        retranslateUi(ViewerWindow);

        QMetaObject::connectSlotsByName(ViewerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ViewerWindow)
    {
        ViewerWindow->setWindowTitle(QApplication::translate("ViewerWindow", "ViewerWindow", 0));
        actionAbrir->setText(QApplication::translate("ViewerWindow", "Abrir", 0));
        actionAbrir->setShortcut(QApplication::translate("ViewerWindow", "Ctrl+A", 0));
        actionSalir->setText(QApplication::translate("ViewerWindow", "Salir", 0));
        actionAcerca_de->setText(QApplication::translate("ViewerWindow", "Acerca de...", 0));
        actionCapturar->setText(QApplication::translate("ViewerWindow", "Capturar", 0));
        actionPreferencias->setText(QApplication::translate("ViewerWindow", "Preferencias", 0));
        actionCaptura_de_red->setText(QApplication::translate("ViewerWindow", "Captura de red", 0));
        stopbutton->setText(QApplication::translate("ViewerWindow", "Stop", 0));
        Quit->setText(QApplication::translate("ViewerWindow", "Salir", 0));
        pause->setText(QApplication::translate("ViewerWindow", "Pause", 0));
        playbutton->setText(QApplication::translate("ViewerWindow", "Play", 0));
        reproducir->setText(QApplication::translate("ViewerWindow", "Reproducir", 0));
        label->setText(QString());
        menuArchivo->setTitle(QApplication::translate("ViewerWindow", "&Archivo", 0));
        menuAcerca_de->setTitle(QApplication::translate("ViewerWindow", "Ayuda", 0));
        menuEditar->setTitle(QApplication::translate("ViewerWindow", "Editar", 0));
    } // retranslateUi

};

namespace Ui {
    class ViewerWindow: public Ui_ViewerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWERWINDOW_H
