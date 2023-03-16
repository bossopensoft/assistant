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

#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QtWidgets/QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QNetworkAccessManager>
#include "syssettingdlg.h"

enum prettyproname
{
    PRETTY_PRODUCT_NAME_UOS,
    PRETTY_PRODUCT_NAME_KYLIN,
    PRETTY_PRODUCT_NAME_NEOKYLIN,
    PRETTY_PRODUCT_NAME_OTHER,
};

enum updateModuleStatus
{
    UPDATE_MODULE_SUCCESS = 0,
    UPDATE_MODULE_UNZIP_ERROR = 1,
    UPDATE_MODULE_FILE_NOT_EXIST = 2,
    UPDATE_MODULE_CMD_ERROR = 3,
    UPDATE_MODULE_UPGRADE_INFO_ERROR = 4,
};

class QTimer;
class AboutDialog;
class SystemTray : public QWidget
{
    Q_OBJECT
public:
    explicit SystemTray(QWidget *parent = nullptr);
    ~SystemTray();

    void startUpdateTimer(bool isFirst = false);
    void  getPrettyProInfo();
protected:
    void activeTray(QSystemTrayIcon::ActivationReason reason);
    void popupPrinterConfigDialog();      // 显示窗体
    void showMessage();     // 消息框
    void showMenu();        // 显示菜单
    void exitApp();
    void restartApp();
    void showSysSettingWindow();
    void about();           //关于
    void updateMenu();

    void updateTimer();
private:
    void update(bool remindIfNoUpgrade = true);          //检测更新
    void updateModule(bool remindIfNoUpgrade = true);    //增量升级
    void updatePackage(const QString& packagePath, const QString& packageName);
    updateModuleStatus copyModuleFile(const QString& info);
private slots:
    void updateFinshed();
    void updateModuleFinshed();
private:
    QSystemTrayIcon *m_systemTray;

    QMenu*      m_menu;
    QAction*    m_action1;
    QAction*    m_action2;
    QAction*    m_action3;
    QAction*    m_action4;
    QAction*    m_action5;
    QAction*    m_action6;
    sysSettingDlg* dSysSetting;
    AboutDialog* m_aboutDlg = nullptr;
    QTimer* m_updateTimer;

    QNetworkAccessManager m_networkAccessManager;
    prettyproname m_prettyProNameFlag;
    QString m_prettyProName;
};

#endif // SYSTEMTRAY_H
