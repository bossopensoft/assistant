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

#ifndef BUSINESSSERVICE_H
#define BUSINESSSERVICE_H

#include <QString>
#include <QSslKey>
#include <QSslCertificate>

const QString MODULE_HEART              = "heart";      // 心跳
const QString MODULE_PREVIEW_IMAGE      = "preview";    // own
const QString MODULE_SIGN               = "sign";       // 兼容旧客户端
const QString MODULE_UPDATE             = "update";     // 客户端更新地址
const QString MODULE_PRINT              = "print";      // 打印

const QString MODULE_UPLOAD             = "upload";     // 上传
const QString MODULE_CA                 = "ca";         // 签名
const QString MODULE_POS                = "pos";        // pos刷卡
const QString MODULE_PD                 = "pd";         //
const QString MODULE_BSREMOTE           = "NontaxAgencyActuator";
const QString MODULE_CAMERA             = "camera";     // 高拍仪
const QString MODULE_APPCTRL            = "appctrl";    // 与客户端服务交互
const QString MODULE_SMART_CARD         = "smart-card"; // 智能卡
const QString MODULE_WEBBILLCARD        = "webbillcard";// WEB票据智能卡
const QString MODULE_EXTEND             = "extend"; // 通用扩展接口
const QString MODULE_QRCODEPAY          = "qrcodepay"; // 扫码支付终端
const QString MODULE_BROWSEREXTEND      = "browser-extend"; //浏览器通用拓展接口

// key
const QString KEY_APP_ID                = "appId"; // own
const QString KEY_UPDATE_URL            = "updateUrl"; // own
const QString KEY_DATA                  = "data"; // own
const QString KEY_CONFIG                = "config"; // own
const QString KEY_APP_FILE_DIR          = "appFileDir"; // own
const QString VALUE_NIL                 = "nil"; // own
const QString KEY_APP_VERSION           = "application.current.version";
const QString KEY_APP_NAME              = "application.name";
const QString KEY_APP_ICON              = "application.icon";
const QString KEY_APP_MENU_PRINTSET     = "application.printSetMenu";
const QString KEY_APP_UPDATE_INTERVAL   = "application.upgrade.timer.interval";
const QString KEY_APP_UPGRADE_CMD_UOS   = "application.upgrade.cmd.uos";
const QString KEY_APP_UPGRADE_CMD_KYLIN = "application.upgrade.cmd.kylin";
const QString KEY_APP_UPGRADE_CMD_NEOKYLIN = "application.upgrade.cmd.neokylin";

const QString KEY_HTTP_UPDATE_URL       = "server.update.url";
const QString KEY_JSONP                 = "jsonp";
const QString KEY_RET_CODE              = "ret_code";
const QString KEY_RET_MSG               = "ret_msg";
const QString KEY_SERVER_IP             = "server.ip";
const QString KEY_SERVER_PORT           = "server.port";
const QString KEY_HTTPS_SERVER_PORT     = "server.https.port";
const QString SYS_INI_HTTP_MODULE       = "HTTP";
const QString KEY_SYS_CERT_PATH         = "server.sslCertFile";
const QString KEY_SYS_CERT_PSW          = "server.sslCertPsw";

//http request key
const QString REQUEST_PARAM_KEY_ID          = "id";
const QString REQUEST_PARAM_KEY_FUNC        = "func";
const QString REQUEST_PARAM_KEY_DOUBLEBASE  = "isDoubleBase";
const QString REQUEST_PARAM_KEY_PAYLOAD     = "payload";


// print configure
const QString NODE_MODULE_PRINTER       = "_PRINTER";
const QString NODE_PRINTER              = "00000000_PRINTER";
const QString DEFAULT_MODULE_ID         = "00000000";
const QString DEFAULT_MODULE_MARGIN     = "0";
const QString DEFAULT_PRINTER_PAPER     = "(自定义纸张)";

// HTTP configure
const QString DEFAULT_SERVER_IP         = "127.0.0.1";
const QString DEFAULT_HTTP_SERVER_PORT  = "13526";
const QString DEFAULT_HTTPS_SERVER_PORT = "13525";

// 错误定义
const QString BSERROR_NO_SERVCONF       = "100";   // 更新检测url为空
const QString BSERROR_RESULT            = "-1";    // 错误返回值
const QString BSERROR_VALUE_NIL         = "nil";


//const QString PRINT_RELY_DLL            = "/lib/boss-report/bossreport_d";
const QString PRINT_RELY_DLL            = "/DLL/3rdparty/boss-report/bossreport";
const QString SYS_INI_PATH              = "/conf/sys.ini";
const QString DEFAULT_VALUE_APP_NAME    = "财政客户端综合管理组件";

const QString UOS_PRETTY_PRO_NAME             = "UnionTech OS";
const QString KYLIN_PRETTY_PRO_NAME           = "Kylin";
const QString NEOKYLIN_PRETTY_PRO_NAME        = "NeoKylin Linux";
const QString DEFAULT_CA_UPDATE_PATH          = "DLL/ca";
const QString DEFAULT_CAMERA_UPDATE_PATH      = "DLL/camera";
const QString DEFAULT_PD_UPDATE_PATH          = "DLL/pd";
const QString DEFAULT_POS_UPDATE_PATH         = "DLL/pos";
const QString DEFAULT_PRINT_UPDATE_PATH       = "DLL/print";
const QString DEFAULT_UPLOAD_UPDATE_PATH      = "DLL/upload";
const QString DEFAULT_NONTAXAGENCYACTUATOR_UPDATE_PATH = "DLL/NontaxAgencyActuator";
const QString DEFAULT_SMART_CARD_UPDATE_PATH  = "DLL/smart-card";
const QString DEFAULT_QRCODEPAY_UPDATE_PATH   = "DLL/qrcodepay";
const QString DEFAULT_BOSS_REPORT_UPDATE_PATH = "DLL/3rdparty/boss-report";

class BusinessService
{
public:
    BusinessService();

    QByteArray serviceEntry(QString appFileDir, QByteArray payload, QString module, QString func);
    QString buildCommonConfig(QByteArray payload, QString appFileDir);
    QString buildCommonData(QByteArray payload);
    QByteArray doBase64Decode(QByteArray src, int decMode);
    QByteArray doBase64Encode(QByteArray src, int encMode);
    QByteArray setJsonString(QString retCode, QString retMsg);
    QString readIniDll(QString module, QString key, QString defaultValue);
    QString readIniSys(QString module, QString key, QString defaultValue);
    void writeIniSys(QString module, QString key, QString value);
    void initCommuParams(void);
    bool isAgency3Intfc(const QString& payload);
    QByteArray ReadScriptText();
    QByteArray BuildResponseScript(const QString& id, const QByteArray& textCode, const QByteArray& res);
    void initService(void);
    void loadRelyDll(void);
    void buildTemplateJsonFile(void);
    bool initSslCertFromFile(QSslKey &key, QSslCertificate &certificate, QList<QSslCertificate> &certChain);
    bool isUseHttps(void);
};

#endif // BUSINESSSERVICE_H
