/********************************************************************************
** Form generated from reading UI file 'ayuda.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AYUDA_H
#define UI_AYUDA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Ayuda
{
public:
    QWidget *verticalLayoutWidget;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer;
    QFrame *line_2;
    QPushButton *Cerrar;
    QSpacerItem *verticalSpacer;
    QLabel *Basado;
    QSpacerItem *verticalSpacer_2;
    QLabel *Desarrollado;
    QLabel *CopyRight;
    QLabel *label;

    void setupUi(QDialog *Ayuda)
    {
        if (Ayuda->objectName().isEmpty())
            Ayuda->setObjectName(QStringLiteral("Ayuda"));
        Ayuda->resize(426, 209);
        verticalLayoutWidget = new QWidget(Ayuda);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(30, 20, 377, 161));
        gridLayout = new QGridLayout(verticalLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 9, 0, 1, 1);

        line_2 = new QFrame(verticalLayoutWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 5, 0, 1, 1);

        Cerrar = new QPushButton(verticalLayoutWidget);
        Cerrar->setObjectName(QStringLiteral("Cerrar"));

        gridLayout->addWidget(Cerrar, 9, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 7, 0, 1, 1);

        Basado = new QLabel(verticalLayoutWidget);
        Basado->setObjectName(QStringLiteral("Basado"));

        gridLayout->addWidget(Basado, 3, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 4, 0, 1, 1);

        Desarrollado = new QLabel(verticalLayoutWidget);
        Desarrollado->setObjectName(QStringLiteral("Desarrollado"));

        gridLayout->addWidget(Desarrollado, 6, 0, 1, 1);

        CopyRight = new QLabel(verticalLayoutWidget);
        CopyRight->setObjectName(QStringLiteral("CopyRight"));

        gridLayout->addWidget(CopyRight, 8, 0, 1, 1);

        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setTextFormat(Qt::AutoText);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(false);

        gridLayout->addWidget(label, 1, 0, 1, 1);


        retranslateUi(Ayuda);

        QMetaObject::connectSlotsByName(Ayuda);
    } // setupUi

    void retranslateUi(QDialog *Ayuda)
    {
        Ayuda->setWindowTitle(QApplication::translate("Ayuda", "Dialog", 0));
        Cerrar->setText(QApplication::translate("Ayuda", "Cerrar", 0));
        Basado->setText(QApplication::translate("Ayuda", "Basado en Qt 5.2.0 (GCC 4.6.1,32 bit)", 0));
        Desarrollado->setText(QApplication::translate("Ayuda", "Desarrollado el 11 de Febrero del 2014 \n"
" por Juan Jos\303\251 Gregorio D\303\255az Marrero", 0));
        CopyRight->setText(QApplication::translate("Ayuda", "Copyright 2014. Todos los derechos \n"
" reservados", 0));
        label->setText(QApplication::translate("Ayuda", "Visor 1.0", 0));
    } // retranslateUi

};

namespace Ui {
    class Ayuda: public Ui_Ayuda {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AYUDA_H
