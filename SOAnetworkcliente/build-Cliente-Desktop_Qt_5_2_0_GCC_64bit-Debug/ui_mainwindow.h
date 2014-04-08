/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

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

class Ui_MainWindow
{
public:
    QAction *actionAbrir;
    QAction *actionSalir;
    QAction *actionAcerca_de_Visor;
    QAction *actionCapturar;
    QAction *actionPreferencias;
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *Pausa;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QPushButton *Boton_Cerrar;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QPushButton *Reproducir;
    QPushButton *Parar;
    QCheckBox *chk_reproducir;
    QWidget *widget;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menuAyuda;
    QMenu *menuCamara;
    QToolBar *toolBar;
    QToolBar *toolBar_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(544, 330);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setAutoFillBackground(true);
        actionAbrir = new QAction(MainWindow);
        actionAbrir->setObjectName(QStringLiteral("actionAbrir"));
        actionSalir = new QAction(MainWindow);
        actionSalir->setObjectName(QStringLiteral("actionSalir"));
        actionAcerca_de_Visor = new QAction(MainWindow);
        actionAcerca_de_Visor->setObjectName(QStringLiteral("actionAcerca_de_Visor"));
        actionCapturar = new QAction(MainWindow);
        actionCapturar->setObjectName(QStringLiteral("actionCapturar"));
        actionPreferencias = new QAction(MainWindow);
        actionPreferencias->setObjectName(QStringLiteral("actionPreferencias"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setContextMenuPolicy(Qt::DefaultContextMenu);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(40, 20, 441, 231));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        Pausa = new QPushButton(layoutWidget);
        Pausa->setObjectName(QStringLiteral("Pausa"));
        Pausa->setEnabled(false);

        gridLayout->addWidget(Pausa, 4, 5, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 6, 0, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(16, 16));
        label->setScaledContents(true);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        Boton_Cerrar = new QPushButton(layoutWidget);
        Boton_Cerrar->setObjectName(QStringLiteral("Boton_Cerrar"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Boton_Cerrar->sizePolicy().hasHeightForWidth());
        Boton_Cerrar->setSizePolicy(sizePolicy2);
        Boton_Cerrar->setSizeIncrement(QSize(0, 0));
        Boton_Cerrar->setLayoutDirection(Qt::LeftToRight);

        gridLayout->addWidget(Boton_Cerrar, 6, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 6, 5, 1, 1);

        verticalSpacer = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);

        Reproducir = new QPushButton(layoutWidget);
        Reproducir->setObjectName(QStringLiteral("Reproducir"));
        Reproducir->setEnabled(false);

        gridLayout->addWidget(Reproducir, 4, 0, 1, 1);

        Parar = new QPushButton(layoutWidget);
        Parar->setObjectName(QStringLiteral("Parar"));
        Parar->setEnabled(false);

        gridLayout->addWidget(Parar, 4, 1, 1, 1);

        chk_reproducir = new QCheckBox(layoutWidget);
        chk_reproducir->setObjectName(QStringLiteral("chk_reproducir"));

        gridLayout->addWidget(chk_reproducir, 2, 0, 1, 1);

        widget = new QWidget(layoutWidget);
        widget->setObjectName(QStringLiteral("widget"));

        gridLayout->addWidget(widget, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 544, 20));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        menuAyuda = new QMenu(menuBar);
        menuAyuda->setObjectName(QStringLiteral("menuAyuda"));
        menuCamara = new QMenu(menuBar);
        menuCamara->setObjectName(QStringLiteral("menuCamara"));
        MainWindow->setMenuBar(menuBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar_2 = new QToolBar(MainWindow);
        toolBar_2->setObjectName(QStringLiteral("toolBar_2"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_2);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menuAyuda->menuAction());
        menuBar->addAction(menuCamara->menuAction());
        menu_File->addSeparator();
        menu_File->addSeparator();
        menu_File->addAction(actionAbrir);
        menu_File->addSeparator();
        menu_File->addAction(actionSalir);
        menuAyuda->addAction(actionAcerca_de_Visor);
        menuCamara->addAction(actionCapturar);
        menuCamara->addAction(actionPreferencias);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Visor", 0));
        actionAbrir->setText(QApplication::translate("MainWindow", "Abrir", 0));
        actionSalir->setText(QApplication::translate("MainWindow", "Salir", 0));
        actionAcerca_de_Visor->setText(QApplication::translate("MainWindow", "Acerca de Visor...", 0));
        actionCapturar->setText(QApplication::translate("MainWindow", "Capturar", 0));
        actionPreferencias->setText(QApplication::translate("MainWindow", "Preferencias", 0));
        Pausa->setText(QApplication::translate("MainWindow", "Pausa", 0));
        label->setText(QString());
        Boton_Cerrar->setText(QApplication::translate("MainWindow", "Cerrar", 0));
        Reproducir->setText(QApplication::translate("MainWindow", "Reproducir", 0));
        Parar->setText(QApplication::translate("MainWindow", "Parar", 0));
        chk_reproducir->setText(QApplication::translate("MainWindow", "Auto-Reproducir", 0));
        menu_File->setTitle(QApplication::translate("MainWindow", "&Documento", 0));
        menuAyuda->setTitle(QApplication::translate("MainWindow", "&Ayuda", 0));
        menuCamara->setTitle(QApplication::translate("MainWindow", "&Camara", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
        toolBar_2->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
