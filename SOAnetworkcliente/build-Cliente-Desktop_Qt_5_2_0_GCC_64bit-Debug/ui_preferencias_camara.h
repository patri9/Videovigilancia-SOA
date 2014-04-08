/********************************************************************************
** Form generated from reading UI file 'preferencias_camara.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCIAS_CAMARA_H
#define UI_PREFERENCIAS_CAMARA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Preferencias_Camara
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QComboBox *Combo_disp;
    QPushButton *pushButton;

    void setupUi(QDialog *Preferencias_Camara)
    {
        if (Preferencias_Camara->objectName().isEmpty())
            Preferencias_Camara->setObjectName(QStringLiteral("Preferencias_Camara"));
        Preferencias_Camara->resize(357, 108);
        verticalLayout = new QVBoxLayout(Preferencias_Camara);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(Preferencias_Camara);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 1, 1, 1);

        Combo_disp = new QComboBox(Preferencias_Camara);
        Combo_disp->setObjectName(QStringLiteral("Combo_disp"));
        Combo_disp->setEditable(false);

        gridLayout->addWidget(Combo_disp, 1, 1, 1, 1);

        pushButton = new QPushButton(Preferencias_Camara);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        gridLayout->addWidget(pushButton, 2, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);


        retranslateUi(Preferencias_Camara);

        QMetaObject::connectSlotsByName(Preferencias_Camara);
    } // setupUi

    void retranslateUi(QDialog *Preferencias_Camara)
    {
        Preferencias_Camara->setWindowTitle(QApplication::translate("Preferencias_Camara", "Dialog", 0));
        label->setText(QApplication::translate("Preferencias_Camara", "Dispositivos ", 0));
        Combo_disp->clear();
        Combo_disp->insertItems(0, QStringList()
         << QApplication::translate("Preferencias_Camara", "Predeterminado", 0)
        );
        pushButton->setText(QApplication::translate("Preferencias_Camara", "Cerrar", 0));
    } // retranslateUi

};

namespace Ui {
    class Preferencias_Camara: public Ui_Preferencias_Camara {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCIAS_CAMARA_H
