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

#ifndef SYSSETTINGDLG_H
#define SYSSETTINGDLG_H

#include <QDialog>

namespace Ui {
class sysSettingDlg;
}

class sysSettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit sysSettingDlg(QWidget *parent = nullptr);
    ~sysSettingDlg();
    void initSysSettingDlg(void);
signals:
    void resetUpdateTimer(bool isFirst);
private:
    Ui::sysSettingDlg *ui;
    void onAutoStartCheckBoxCliked();
    void onUpIntervalChanged();
    void onSaveSettingProcess();
    QString appName;
private slots:
    void onComfirmBtnClicked(void);
    void onCencelBtnClicked(void);
};

#endif // SYSSETTINGDLG_H
