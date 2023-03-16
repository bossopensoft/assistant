/***************************************************************************
 *   This file is part of the Bosssoft Assistant Client Project                          *
 *   Copyright (C) 2023 by Bin Yan                                                                *
 *   yanbin@bosssoft.com.cn                                                                        *
 *                                                                                                                   *
 **                   GNU General Public License Usage                                      **
 *                                                                                                                  *
 *   This library is free software: you can redistribute it and/or modify        *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or          *
 *   (at your option) any later version.                                                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                                                                   *
 **                  GNU Lesser General Public License                                      **
 *                                                                                                                   *
 *   This library is free software: you can redistribute it and/or modify         *
 *   it under the terms of the GNU Lesser General Public License as             *
 *   published by the Free Software Foundation, either version 3 of the       *
 *   License, or (at your option) any later version.                                         *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with this library.                                                                *
 *   If not, see <http://www.gnu.org/licenses/>.                                            *
 *                                                                                                                   *
 *   This library is distributed in the hope that it will be useful,                     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the   *
 *   GNU General Public License for more details.                                          *
 ****************************************************************************/

#ifndef PRINTINGCONFIGDIALOG_H
#define PRINTINGCONFIGDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QtPrintSupport/QPrinterInfo>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class PrintingConfigDialog; }
QT_END_NAMESPACE


class QMargins;
class PaperOffset : protected QPair<int,int>
{
public:
    PaperOffset(const int horizontial=0, const int vertical=0):QPair(horizontial,vertical){}
    int Horizontial()const{return this->first;}
    int Vertical()const{return this->second;}
};

class PrintingConfigDialog : public QDialog
{
    Q_OBJECT

public:
    PrintingConfigDialog(QWidget *parent = nullptr);
    ~PrintingConfigDialog();
    void updatePrinterParams(void); //设置项不包含moduleId和moduleName
    void onSaveAllParams(void);
    void initPrinterDialog(void);

public slots:
    void onCancelPressed();
    void onOKPressed();

protected:
    virtual void showEvent(QShowEvent *event);

private slots:
    void onPrinterChanged(int index);
    void onModuleIdChanged(int index);
private:
    void initDefaultModuleNumbers();

    void initDefaultPrinters();
    void updatePaperSize();
    void updatePaperDirection();

    Ui::PrintingConfigDialog *ui;

    QString     mAppId;
    QStringList mModuleNumbers;
    QStringList mModuleName;
    QStringList mPrinters;
    QStringList mPaperSizes;
    QStringList mPaperDirection;
    QMargins    mPaperMargins;
    PaperOffset mPaperOffset;

    QList<QPrinterInfo> mSupportingPrinters;
    int                 mCurrentPrinterIdx;
    int                 mCurrentModuleSNIdx;
    QMap<QString, QString>mMapModule;
};
#endif // PRINTINGCONFIGDIALOG_H
