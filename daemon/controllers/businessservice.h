#ifndef BUSINESSSERVICE_H
#define BUSINESSSERVICE_H

#include <QString>
#include <QSslKey>
#include <QSslCertificate>

const QString MODULE_APPCTRL            = "controlMainApp";    // 客户端控制
const QString MODULE_UPGRADE            = "upgradeMainApp";    // 客户端升级

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
const QString KEY_HTTP_UPDATE_URL        = "server.update.url";
const QString KEY_JSONP                 = "jsonp";
const QString KEY_RET_CODE              = "ret_code";
const QString KEY_RET_MSG               = "ret_msg";
const QString KEY_SERVER_IP             = "server.ip";
const QString KEY_SERVER_PORT           = "server.port";
const QString KEY_HTTPS_SERVER_PORT     = "server.https.port";
const QString SYS_INI_HTTP_MODULE       =   "HTTP";
const QString KEY_SYS_CERT_PATH         =   "server.sslCertFile";
const QString KEY_SYS_CERT_PSW          =   "server.sslCertPsw";

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


class BusinessService
{
public:
    BusinessService();

    QByteArray setJsonString(QString retCode, QString retMsg);
    QString readIniDll(QString module, QString key, QString defaultValue);
    QString readIniSys(QString module, QString key, QString defaultValue);
    bool initSslCertFromFile(QSslKey &key, QSslCertificate &certificate, QList<QSslCertificate> &certChain);
    bool isUseHttps(void);
};

#endif // BUSINESSSERVICE_H
