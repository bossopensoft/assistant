/********************************************************************************
** Form generated from reading UI file 'syssettingdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYSSETTINGDLG_H
#define UI_SYSSETTINGDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_sysSettingDlg
{
public:
    QPushButton *comfirmButton;
    QGroupBox *groupBox;
    QLabel *serverPortNameLabel;
    QLabel *serverAddrNameLable;
    QCheckBox *autoStartCheckBox;
    QLabel *autoUpIntervalLabel;
    QLabel *hourLabel;
    QLabel *serverAddrContentLabel;
    QLabel *serverPortContentLabel;
    QLineEdit *timelineEdit;
    QPushButton *cancelButton;

    void setupUi(QDialog *sysSettingDlg)
    {
        if (sysSettingDlg->objectName().isEmpty())
            sysSettingDlg->setObjectName(QString::fromUtf8("sysSettingDlg"));
        sysSettingDlg->resize(460, 260);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sysSettingDlg->sizePolicy().hasHeightForWidth());
        sysSettingDlg->setSizePolicy(sizePolicy);
        sysSettingDlg->setMinimumSize(QSize(460, 260));
        sysSettingDlg->setMaximumSize(QSize(460, 260));
        comfirmButton = new QPushButton(sysSettingDlg);
        comfirmButton->setObjectName(QString::fromUtf8("comfirmButton"));
        comfirmButton->setGeometry(QRect(370, 40, 71, 36));
        groupBox = new QGroupBox(sysSettingDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 20, 331, 221));
        serverPortNameLabel = new QLabel(groupBox);
        serverPortNameLabel->setObjectName(QString::fromUtf8("serverPortNameLabel"));
        serverPortNameLabel->setGeometry(QRect(40, 70, 71, 22));
        serverAddrNameLable = new QLabel(groupBox);
        serverAddrNameLable->setObjectName(QString::fromUtf8("serverAddrNameLable"));
        serverAddrNameLable->setGeometry(QRect(40, 30, 71, 22));
        autoStartCheckBox = new QCheckBox(groupBox);
        autoStartCheckBox->setObjectName(QString::fromUtf8("autoStartCheckBox"));
        autoStartCheckBox->setGeometry(QRect(130, 120, 171, 26));
        autoUpIntervalLabel = new QLabel(groupBox);
        autoUpIntervalLabel->setObjectName(QString::fromUtf8("autoUpIntervalLabel"));
        autoUpIntervalLabel->setGeometry(QRect(40, 180, 131, 22));
        hourLabel = new QLabel(groupBox);
        hourLabel->setObjectName(QString::fromUtf8("hourLabel"));
        hourLabel->setGeometry(QRect(230, 180, 63, 22));
        serverAddrContentLabel = new QLabel(groupBox);
        serverAddrContentLabel->setObjectName(QString::fromUtf8("serverAddrContentLabel"));
        serverAddrContentLabel->setGeometry(QRect(130, 30, 161, 22));
        serverPortContentLabel = new QLabel(groupBox);
        serverPortContentLabel->setObjectName(QString::fromUtf8("serverPortContentLabel"));
        serverPortContentLabel->setGeometry(QRect(130, 70, 161, 22));
        timelineEdit = new QLineEdit(groupBox);
        timelineEdit->setObjectName(QString::fromUtf8("timelineEdit"));
        timelineEdit->setGeometry(QRect(160, 180, 61, 21));
        cancelButton = new QPushButton(sysSettingDlg);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(370, 100, 71, 36));

        retranslateUi(sysSettingDlg);

        QMetaObject::connectSlotsByName(sysSettingDlg);
    } // setupUi

    void retranslateUi(QDialog *sysSettingDlg)
    {
        sysSettingDlg->setWindowTitle(QApplication::translate("sysSettingDlg", "\347\263\273\347\273\237\350\256\276\347\275\256", nullptr));
        comfirmButton->setText(QApplication::translate("sysSettingDlg", "\347\241\256\345\256\232", nullptr));
        groupBox->setTitle(QString());
        serverPortNameLabel->setText(QApplication::translate("sysSettingDlg", "\346\234\215\345\212\241\347\253\257\345\217\243", nullptr));
        serverAddrNameLable->setText(QApplication::translate("sysSettingDlg", "\346\234\215\345\212\241\345\234\260\345\235\200", nullptr));
        autoStartCheckBox->setText(QApplication::translate("sysSettingDlg", "\345\274\200\346\234\272\346\227\266\350\207\252\345\212\250\345\220\257\345\212\250\345\256\242\346\210\267\347\253\257", nullptr));
        autoUpIntervalLabel->setText(QApplication::translate("sysSettingDlg", "\350\207\252\345\212\250\346\233\264\346\226\260\346\227\266\351\227\264\351\227\264\351\232\224", nullptr));
        hourLabel->setText(QApplication::translate("sysSettingDlg", "\346\227\266", nullptr));
        serverAddrContentLabel->setText(QApplication::translate("sysSettingDlg", "127.0.0.1", nullptr));
        serverPortContentLabel->setText(QApplication::translate("sysSettingDlg", "13526", nullptr));
        cancelButton->setText(QApplication::translate("sysSettingDlg", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sysSettingDlg: public Ui_sysSettingDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSSETTINGDLG_H
