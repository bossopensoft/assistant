/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QLabel *clientIconLabel;
    QLabel *clientNameLabel;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *pushButton;
    QTableView *tableView;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->setWindowModality(Qt::NonModal);
        AboutDialog->resize(500, 300);
        AboutDialog->setMinimumSize(QSize(500, 300));
        AboutDialog->setMaximumSize(QSize(500, 300));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/SysTrayIcon/Resources/ico.png"), QSize(), QIcon::Normal, QIcon::Off);
        AboutDialog->setWindowIcon(icon);
        AboutDialog->setLayoutDirection(Qt::LeftToRight);
        AboutDialog->setModal(false);
        clientIconLabel = new QLabel(AboutDialog);
        clientIconLabel->setObjectName(QString::fromUtf8("clientIconLabel"));
        clientIconLabel->setGeometry(QRect(40, 20, 81, 91));
        clientIconLabel->setPixmap(QPixmap(QString::fromUtf8(":/SysTrayIcon/Resources/logo.png")));
        clientIconLabel->setScaledContents(true);
        clientNameLabel = new QLabel(AboutDialog);
        clientNameLabel->setObjectName(QString::fromUtf8("clientNameLabel"));
        clientNameLabel->setGeometry(QRect(160, 20, 331, 41));
        label_3 = new QLabel(AboutDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(160, 70, 311, 21));
        label_4 = new QLabel(AboutDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 219, 481, 41));
        label_4->setAlignment(Qt::AlignCenter);
        pushButton = new QPushButton(AboutDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(384, 260, 91, 31));
        tableView = new QTableView(AboutDialog);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(160, 100, 311, 111));
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setShowGrid(false);
        tableView->setGridStyle(Qt::SolidLine);
        tableView->horizontalHeader()->setVisible(false);
        tableView->verticalHeader()->setVisible(false);

        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "\345\205\263\344\272\216", nullptr));
        clientIconLabel->setText(QString());
        clientNameLabel->setText(QApplication::translate("AboutDialog", "\350\264\242\346\224\277\347\273\274\345\220\210\347\256\241\347\220\206\347\273\204\344\273\266\345\256\242\346\210\267\347\253\257", nullptr));
        label_3->setText(QApplication::translate("AboutDialog", "\347\273\204\344\273\266\347\211\210\346\234\254\357\274\232", nullptr));
        label_4->setText(QApplication::translate("AboutDialog", "\347\211\210\346\235\203\346\211\200\346\234\211 \302\251 2021\357\274\214\344\277\235\347\225\231\346\211\200\346\234\211\346\235\203\345\210\251\343\200\202", nullptr));
        pushButton->setText(QApplication::translate("AboutDialog", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
