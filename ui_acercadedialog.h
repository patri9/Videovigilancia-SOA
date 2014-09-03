/********************************************************************************
** Form generated from reading UI file 'acercadedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACERCADEDIALOG_H
#define UI_ACERCADEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AcercaDeDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AcercaDeDialog)
    {
        if (AcercaDeDialog->objectName().isEmpty())
            AcercaDeDialog->setObjectName(QStringLiteral("AcercaDeDialog"));
        AcercaDeDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(AcercaDeDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(AcercaDeDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setLayoutDirection(Qt::LeftToRight);

        verticalLayout->addWidget(label);

        buttonBox = new QDialogButtonBox(AcercaDeDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(AcercaDeDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AcercaDeDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AcercaDeDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AcercaDeDialog);
    } // setupUi

    void retranslateUi(QDialog *AcercaDeDialog)
    {
        AcercaDeDialog->setWindowTitle(QApplication::translate("AcercaDeDialog", "Dialog", 0));
        label->setText(QApplication::translate("AcercaDeDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'.Lucida Grande UI'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:600;\">Patricia Garc\303\255a Rodr\303\255guez</span></p>\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Sistemas Operativos Avanzados</p>\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Versi\303\263n 0.1</p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class AcercaDeDialog: public Ui_AcercaDeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACERCADEDIALOG_H
