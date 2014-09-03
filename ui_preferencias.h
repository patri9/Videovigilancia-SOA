/********************************************************************************
** Form generated from reading UI file 'preferencias.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCIAS_H
#define UI_PREFERENCIAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_Preferencias
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *aceptar;
    QPushButton *cancelar;
    QComboBox *comboBox;

    void setupUi(QDialog *Preferencias)
    {
        if (Preferencias->objectName().isEmpty())
            Preferencias->setObjectName(QStringLiteral("Preferencias"));
        Preferencias->resize(453, 335);
        gridLayout = new QGridLayout(Preferencias);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 3, 0, 1, 1);

        aceptar = new QPushButton(Preferencias);
        aceptar->setObjectName(QStringLiteral("aceptar"));

        gridLayout->addWidget(aceptar, 3, 1, 1, 1);

        cancelar = new QPushButton(Preferencias);
        cancelar->setObjectName(QStringLiteral("cancelar"));

        gridLayout->addWidget(cancelar, 3, 2, 1, 1);

        comboBox = new QComboBox(Preferencias);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        gridLayout->addWidget(comboBox, 0, 0, 1, 3);


        retranslateUi(Preferencias);

        QMetaObject::connectSlotsByName(Preferencias);
    } // setupUi

    void retranslateUi(QDialog *Preferencias)
    {
        Preferencias->setWindowTitle(QApplication::translate("Preferencias", "Dialog", 0));
        aceptar->setText(QApplication::translate("Preferencias", "Aceptar", 0));
        cancelar->setText(QApplication::translate("Preferencias", "Cancelar", 0));
    } // retranslateUi

};

namespace Ui {
    class Preferencias: public Ui_Preferencias {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCIAS_H
