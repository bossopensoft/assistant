/********************************************************************************
** Form generated from reading UI file 'printingconfigdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRINTINGCONFIGDIALOG_H
#define UI_PRINTINGCONFIGDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_PrintingConfigDialog
{
public:
    QGroupBox *groupBox_main;
    QLabel *label_moduleSN;
    QLabel *label_moduleName;
    QLabel *label_valueOfModuleName;
    QGroupBox *groupBox_Printer;
    QLabel *label_printerName;
    QLabel *label_paperSize;
    QLabel *label_paperDirection;
    QComboBox *comboBox_paperDirection;
    QComboBox *comboBox_paperSize;
    QComboBox *comboBox_printer;
    QGroupBox *groupBox_margin;
    QLabel *label_topMargin;
    QLabel *label_leftMargin;
    QLabel *label_bottomMargin;
    QLabel *label_rightMargin;
    QLineEdit *lineEdit_valueOfTopMargin;
    QLineEdit *lineEdit_valueOfbottomMargin;
    QLineEdit *lineEdit_valueOfRightMargin;
    QLineEdit *lineEdit_valueOfLeftMargin;
    QGroupBox *groupBox_offset;
    QLabel *label_offsetOfHorizontal;
    QLabel *label_offsetOfVertical;
    QLineEdit *lineEdit_offsetValueOfHorizontal;
    QLineEdit *lineEdit_offsetValueOfVertical;
    QComboBox *comboBox_moduleSN;
    QPushButton *button_OK;
    QPushButton *button_cancel;

    void setupUi(QDialog *PrintingConfigDialog)
    {
        if (PrintingConfigDialog->objectName().isEmpty())
            PrintingConfigDialog->setObjectName(QString::fromUtf8("PrintingConfigDialog"));
        PrintingConfigDialog->resize(800, 638);
        groupBox_main = new QGroupBox(PrintingConfigDialog);
        groupBox_main->setObjectName(QString::fromUtf8("groupBox_main"));
        groupBox_main->setGeometry(QRect(20, 30, 551, 581));
        label_moduleSN = new QLabel(groupBox_main);
        label_moduleSN->setObjectName(QString::fromUtf8("label_moduleSN"));
        label_moduleSN->setGeometry(QRect(10, 20, 101, 41));
        label_moduleSN->setAlignment(Qt::AlignCenter);
        label_moduleName = new QLabel(groupBox_main);
        label_moduleName->setObjectName(QString::fromUtf8("label_moduleName"));
        label_moduleName->setGeometry(QRect(10, 80, 101, 41));
        label_moduleName->setAlignment(Qt::AlignCenter);
        label_valueOfModuleName = new QLabel(groupBox_main);
        label_valueOfModuleName->setObjectName(QString::fromUtf8("label_valueOfModuleName"));
        label_valueOfModuleName->setGeometry(QRect(130, 80, 401, 41));
        groupBox_Printer = new QGroupBox(groupBox_main);
        groupBox_Printer->setObjectName(QString::fromUtf8("groupBox_Printer"));
        groupBox_Printer->setGeometry(QRect(20, 120, 511, 201));
        label_printerName = new QLabel(groupBox_Printer);
        label_printerName->setObjectName(QString::fromUtf8("label_printerName"));
        label_printerName->setGeometry(QRect(10, 30, 81, 41));
        label_printerName->setAlignment(Qt::AlignCenter);
        label_paperSize = new QLabel(groupBox_Printer);
        label_paperSize->setObjectName(QString::fromUtf8("label_paperSize"));
        label_paperSize->setGeometry(QRect(10, 90, 71, 41));
        label_paperSize->setAlignment(Qt::AlignCenter);
        label_paperDirection = new QLabel(groupBox_Printer);
        label_paperDirection->setObjectName(QString::fromUtf8("label_paperDirection"));
        label_paperDirection->setGeometry(QRect(10, 150, 61, 41));
        label_paperDirection->setAlignment(Qt::AlignCenter);
        comboBox_paperDirection = new QComboBox(groupBox_Printer);
        comboBox_paperDirection->setObjectName(QString::fromUtf8("comboBox_paperDirection"));
        comboBox_paperDirection->setGeometry(QRect(100, 150, 391, 41));
        comboBox_paperDirection->setLocale(QLocale(QLocale::Chinese, QLocale::China));
        comboBox_paperSize = new QComboBox(groupBox_Printer);
        comboBox_paperSize->setObjectName(QString::fromUtf8("comboBox_paperSize"));
        comboBox_paperSize->setGeometry(QRect(100, 90, 391, 41));
        comboBox_paperSize->setLocale(QLocale(QLocale::Chinese, QLocale::China));
        comboBox_printer = new QComboBox(groupBox_Printer);
        comboBox_printer->setObjectName(QString::fromUtf8("comboBox_printer"));
        comboBox_printer->setGeometry(QRect(100, 30, 391, 41));
        comboBox_printer->setLocale(QLocale(QLocale::Chinese, QLocale::China));
        groupBox_margin = new QGroupBox(groupBox_main);
        groupBox_margin->setObjectName(QString::fromUtf8("groupBox_margin"));
        groupBox_margin->setGeometry(QRect(20, 330, 511, 141));
        label_topMargin = new QLabel(groupBox_margin);
        label_topMargin->setObjectName(QString::fromUtf8("label_topMargin"));
        label_topMargin->setGeometry(QRect(10, 30, 71, 41));
        label_topMargin->setAlignment(Qt::AlignCenter);
        label_leftMargin = new QLabel(groupBox_margin);
        label_leftMargin->setObjectName(QString::fromUtf8("label_leftMargin"));
        label_leftMargin->setGeometry(QRect(10, 90, 61, 41));
        label_leftMargin->setAlignment(Qt::AlignCenter);
        label_bottomMargin = new QLabel(groupBox_margin);
        label_bottomMargin->setObjectName(QString::fromUtf8("label_bottomMargin"));
        label_bottomMargin->setGeometry(QRect(270, 30, 71, 41));
        label_bottomMargin->setAlignment(Qt::AlignCenter);
        label_rightMargin = new QLabel(groupBox_margin);
        label_rightMargin->setObjectName(QString::fromUtf8("label_rightMargin"));
        label_rightMargin->setGeometry(QRect(270, 90, 61, 41));
        label_rightMargin->setAlignment(Qt::AlignCenter);
        lineEdit_valueOfTopMargin = new QLineEdit(groupBox_margin);
        lineEdit_valueOfTopMargin->setObjectName(QString::fromUtf8("lineEdit_valueOfTopMargin"));
        lineEdit_valueOfTopMargin->setGeometry(QRect(100, 30, 141, 41));
        lineEdit_valueOfbottomMargin = new QLineEdit(groupBox_margin);
        lineEdit_valueOfbottomMargin->setObjectName(QString::fromUtf8("lineEdit_valueOfbottomMargin"));
        lineEdit_valueOfbottomMargin->setGeometry(QRect(360, 30, 141, 41));
        lineEdit_valueOfRightMargin = new QLineEdit(groupBox_margin);
        lineEdit_valueOfRightMargin->setObjectName(QString::fromUtf8("lineEdit_valueOfRightMargin"));
        lineEdit_valueOfRightMargin->setGeometry(QRect(360, 90, 141, 41));
        lineEdit_valueOfLeftMargin = new QLineEdit(groupBox_margin);
        lineEdit_valueOfLeftMargin->setObjectName(QString::fromUtf8("lineEdit_valueOfLeftMargin"));
        lineEdit_valueOfLeftMargin->setGeometry(QRect(100, 90, 141, 41));
        groupBox_offset = new QGroupBox(groupBox_main);
        groupBox_offset->setObjectName(QString::fromUtf8("groupBox_offset"));
        groupBox_offset->setGeometry(QRect(20, 480, 511, 81));
        label_offsetOfHorizontal = new QLabel(groupBox_offset);
        label_offsetOfHorizontal->setObjectName(QString::fromUtf8("label_offsetOfHorizontal"));
        label_offsetOfHorizontal->setGeometry(QRect(10, 30, 71, 41));
        label_offsetOfHorizontal->setAlignment(Qt::AlignCenter);
        label_offsetOfVertical = new QLabel(groupBox_offset);
        label_offsetOfVertical->setObjectName(QString::fromUtf8("label_offsetOfVertical"));
        label_offsetOfVertical->setGeometry(QRect(270, 30, 71, 41));
        label_offsetOfVertical->setAlignment(Qt::AlignCenter);
        lineEdit_offsetValueOfHorizontal = new QLineEdit(groupBox_offset);
        lineEdit_offsetValueOfHorizontal->setObjectName(QString::fromUtf8("lineEdit_offsetValueOfHorizontal"));
        lineEdit_offsetValueOfHorizontal->setGeometry(QRect(100, 30, 141, 41));
        lineEdit_offsetValueOfVertical = new QLineEdit(groupBox_offset);
        lineEdit_offsetValueOfVertical->setObjectName(QString::fromUtf8("lineEdit_offsetValueOfVertical"));
        lineEdit_offsetValueOfVertical->setGeometry(QRect(360, 30, 141, 41));
        comboBox_moduleSN = new QComboBox(groupBox_main);
        comboBox_moduleSN->setObjectName(QString::fromUtf8("comboBox_moduleSN"));
        comboBox_moduleSN->setGeometry(QRect(120, 20, 391, 41));
        comboBox_moduleSN->setLocale(QLocale(QLocale::Chinese, QLocale::China));
        button_OK = new QPushButton(PrintingConfigDialog);
        button_OK->setObjectName(QString::fromUtf8("button_OK"));
        button_OK->setGeometry(QRect(610, 80, 151, 41));
        button_cancel = new QPushButton(PrintingConfigDialog);
        button_cancel->setObjectName(QString::fromUtf8("button_cancel"));
        button_cancel->setGeometry(QRect(610, 170, 151, 41));

        retranslateUi(PrintingConfigDialog);

        comboBox_paperDirection->setCurrentIndex(-1);
        comboBox_paperSize->setCurrentIndex(-1);
        comboBox_printer->setCurrentIndex(-1);
        comboBox_moduleSN->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(PrintingConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *PrintingConfigDialog)
    {
        PrintingConfigDialog->setWindowTitle(QApplication::translate("PrintingConfigDialog", "PrintingConfigDialog", nullptr));
        groupBox_main->setTitle(QString());
        label_moduleSN->setText(QApplication::translate("PrintingConfigDialog", "\346\250\241\345\235\227\347\274\226\345\217\267", nullptr));
        label_moduleName->setText(QApplication::translate("PrintingConfigDialog", "\346\250\241\345\235\227\345\220\215\347\247\260", nullptr));
        label_valueOfModuleName->setText(QString());
        groupBox_Printer->setTitle(QApplication::translate("PrintingConfigDialog", "\346\211\223\345\215\260\346\234\272", nullptr));
        label_printerName->setText(QApplication::translate("PrintingConfigDialog", "\346\211\223\345\215\260\346\234\272", nullptr));
        label_paperSize->setText(QApplication::translate("PrintingConfigDialog", "\347\272\270\345\274\240", nullptr));
        label_paperDirection->setText(QApplication::translate("PrintingConfigDialog", "\346\226\271\345\220\221", nullptr));
        comboBox_paperDirection->setCurrentText(QString());
        comboBox_paperSize->setCurrentText(QString());
        comboBox_printer->setCurrentText(QString());
        groupBox_margin->setTitle(QApplication::translate("PrintingConfigDialog", "\351\241\265\350\276\271\350\267\235\357\274\210\345\216\230\347\261\263\357\274\211", nullptr));
        label_topMargin->setText(QApplication::translate("PrintingConfigDialog", "\344\270\212\350\276\271\350\267\235", nullptr));
        label_leftMargin->setText(QApplication::translate("PrintingConfigDialog", "\345\267\246\350\276\271\350\267\235", nullptr));
        label_bottomMargin->setText(QApplication::translate("PrintingConfigDialog", "\344\270\213\350\276\271\350\267\235", nullptr));
        label_rightMargin->setText(QApplication::translate("PrintingConfigDialog", "\345\217\263\350\276\271\350\267\235", nullptr));
        lineEdit_valueOfTopMargin->setText(QApplication::translate("PrintingConfigDialog", "0", nullptr));
        lineEdit_valueOfbottomMargin->setText(QApplication::translate("PrintingConfigDialog", "0", nullptr));
        lineEdit_valueOfRightMargin->setText(QApplication::translate("PrintingConfigDialog", "0", nullptr));
        lineEdit_valueOfLeftMargin->setText(QApplication::translate("PrintingConfigDialog", "0", nullptr));
        groupBox_offset->setTitle(QApplication::translate("PrintingConfigDialog", "\345\201\217\347\247\273\351\207\217\357\274\210\345\216\230\347\261\263\357\274\211", nullptr));
        label_offsetOfHorizontal->setText(QApplication::translate("PrintingConfigDialog", "\346\250\252\345\220\221\345\201\217\347\247\273", nullptr));
        label_offsetOfVertical->setText(QApplication::translate("PrintingConfigDialog", "\347\272\265\345\220\221\345\201\217\347\247\273", nullptr));
        lineEdit_offsetValueOfHorizontal->setText(QApplication::translate("PrintingConfigDialog", "0", nullptr));
        lineEdit_offsetValueOfVertical->setText(QApplication::translate("PrintingConfigDialog", "0", nullptr));
        comboBox_moduleSN->setCurrentText(QString());
        button_OK->setText(QApplication::translate("PrintingConfigDialog", "\347\241\256\345\256\232", nullptr));
        button_cancel->setText(QApplication::translate("PrintingConfigDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrintingConfigDialog: public Ui_PrintingConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTINGCONFIGDIALOG_H
