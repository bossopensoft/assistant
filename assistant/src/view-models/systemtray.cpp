#include "systemtray.h"
#include <QApplication>
#include <QMessageBox>
#include <QProcess>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QtDebug>
#include <QFile>
#include <QTimer>
#include <QDir>
#include <QThread>
#include <QSettings>
#include "aboutdialog.h"
#include "printingconfigdialog.h"
#include "sysconfigmanager.h"
#include "../controllers/businessservice.h"
#include "../utility/utility.h"
#include "../utility/ziputils.h"
#include "../utility/installsubmodul.h"
#include "inifilehandler.h"
#include "configmanager.h"
#include <QFileInfo>

static QString getAppName(void)
{
    QString appName;
    BusinessService tmpbsService;
    appName = tmpbsService.readIniSys("APP", KEY_APP_NAME, DEFAULT_VALUE_APP_NAME);
    appName = appName.toLatin1();
    return appName;
}

SystemTray::SystemTray(QWidget *parent) : QWidget(parent)
{
    m_systemTray = new QSystemTrayIcon(this);
    m_updateTimer = new QTimer(this);

    m_menu    = new QMenu(this);
    m_action1 = new QAction(m_menu);
    m_action2 = new QAction(m_menu);
    m_action3 = new QAction(m_menu);
    m_action4 = new QAction(m_menu);
    m_action5 = new QAction(m_menu);
    m_action6 = new QAction(m_menu);

    //m_action13 = new QAction(m_menu);

    m_action1->setText("打印设置");
    m_action2->setText("系统设置");
    m_action3->setText("重新启动");
    m_action4->setText("检查更新");
    m_action5->setText("关于");
    m_action6->setText("退出程序");
    //m_action13->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));

    m_menu->addAction(m_action1);
    m_menu->addAction(m_action2);
    m_menu->addAction(m_action3);
    m_menu->addAction(m_action4);
    m_menu->addAction(m_action5);
    m_menu->addAction(m_action6);
    //m_menu->addAction(m_action13);

    connect(m_action1, &QAction::triggered, this, &SystemTray::popupPrinterConfigDialog);
    connect(m_action2, &QAction::triggered, this, &SystemTray::showSysSettingWindow);
    connect(m_action3, &QAction::triggered, this, &SystemTray::restartApp);
    connect(m_action4, &QAction::triggered, this, &SystemTray::updateMenu);
    connect(m_action5, &QAction::triggered, this, &SystemTray::about);
    connect(m_action6, &QAction::triggered, this, &SystemTray::exitApp);

    QString appName = getAppName();

    m_systemTray->setContextMenu(m_menu);
    m_systemTray->setIcon(QIcon(":/SysTrayIcon/Resources/logo_small.png"));
    m_systemTray->setToolTip(appName);
    m_systemTray->show();
    connect(m_systemTray, &QSystemTrayIcon::activated, this, &SystemTray::activeTray);//点击托盘，执行相应的动作
    //connect(m_systemTray, &QSystemTrayIcon::messageClicked, this, &SystemTray::popupPrinterConfigDialog);//点击消息框，显示主窗口

    m_aboutDlg = new AboutDialog(nullptr);

    m_systemTray->showMessage("提示：",//消息窗口标题
        appName,//消息内容
        QIcon(":/SysTrayIcon/Resources/logo_small.png"),//消息窗口图标
        5000);//消息窗口显示时长
    dSysSetting = new sysSettingDlg(nullptr);
    connect(dSysSetting, &sysSettingDlg::resetUpdateTimer, this, &SystemTray::startUpdateTimer);

    InstallSubmoduleThread* m_subThread = new InstallSubmoduleThread;
    QThread* m_objThread = new QThread();
    m_subThread->moveToThread(m_objThread);
    connect(m_objThread,&QThread::finished,m_objThread,&QObject::deleteLater);
    connect(m_subThread, &InstallSubmoduleThread::startUpdateTimer, this, &SystemTray::startUpdateTimer);
    m_objThread->start();
    m_subThread->install();
    //    startUpdateTimer();
    getPrettyProInfo();
}

SystemTray::~SystemTray()
{
    if (m_menu) {
        delete m_menu;
    }

    if (dSysSetting) {
        delete dSysSetting;
    }

    if (m_aboutDlg) {
        delete m_aboutDlg;
    }

    if (m_systemTray) {
        delete m_systemTray;
    }
}

void SystemTray::activeTray(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Context:
        showMenu();
        break;
    /*
    case QSystemTrayIcon::DoubleClick:
        showWindow();
        break;
    case QSystemTrayIcon::Trigger:
        showMessage();
        break;
    */
    default:
        break;
    }
}

void SystemTray::showMenu()
{
    m_menu->show();
}

void SystemTray::popupPrinterConfigDialog()
{
    PrintingConfigDialog _dialog;
    _dialog.initPrinterDialog();
    _dialog.exec();
}

void SystemTray::showSysSettingWindow()
{
    dSysSetting->initSysSettingDlg();
    dSysSetting->show();
}

void SystemTray::showMessage()
{
    m_systemTray->showMessage("Information",//消息窗口标题
        "There is a new message!",//消息内容
        QSystemTrayIcon::MessageIcon::Information,//消息窗口图标
        5000);//消息窗口显示时长
}

void SystemTray::exitApp()
{
    int result;
    QString appName = getAppName();
    //QApplication::setQuitOnLastWindowClosed(false);
    result = QMessageBox::warning(nullptr, appName, tr("确定要退出客户端组件吗？"),tr("是"),tr("否"), nullptr);
    if (result == QObject::tr(("是")).toInt()) {
        QString _filePath =ConfigManager::getInstance()->getSysConfigManager()->filePath();
        QSharedPointer<IniFileHandler> tmpFileHandler(new IniFileHandler(_filePath));
        if (tmpFileHandler->exist()) {
            tmpFileHandler->setValue("/APP/application.exit", "11111");
        }
        qApp->exit(0);
    }

    return;
}

void SystemTray::restartApp()
{
     //QProcess::startDetached(qApp->applicationFilePath(), QStringList());
     qApp->exit(252);
}

void SystemTray::about()
{
    if (m_aboutDlg && m_aboutDlg->isHidden()){
        m_aboutDlg->show();
    }
}

void SystemTray::updateMenu()
{
    update(true);
}

void SystemTray::updateTimer()
{
    update(false);
}

void SystemTray::update(bool remindIfNoUpgrade)
{
    SysConfigManager mng;
    QString appName = getAppName();
    QString updateUrl = mng.getHTTPConnectionInfo().value(KEY_HTTP_UPDATE_URL).toString();
    if (updateUrl.isEmpty()){
        if (remindIfNoUpgrade)
            UTILITY::BsMessagebox(nullptr, appName,tr("已安装最新版本，无需更新！"), tr("确定"), nullptr);
        return;
    }
    QString ver = mng.getAppInfo().value(KEY_APP_VERSION).toString();
    ver = ver.right(ver.length()-1);

    QUrl u = QUrl::fromEncoded(updateUrl.toLocal8Bit());
    QNetworkRequest req(u);
    BusinessService bus;
    if (bus.isUseHttps()) {
        QSslKey key;
        QSslCertificate certificate;
        QList<QSslCertificate> certChain;
        if (!bus.initSslCertFromFile(key, certificate, certChain)) {
            qCritical() << "configure ssl failure";
            return;
        }
        QSslConfiguration config = QSslConfiguration::defaultConfiguration();
        //config.setProtocol(QSsl::TlsV1_2OrLater);
        config.setProtocol(QSsl::AnyProtocol);
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setCaCertificates(certChain);
        config.setLocalCertificate(certificate);
        config.setPrivateKey(key);
        req.setSslConfiguration(config);
    }
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setRawHeader("os", UTILITY::getOs().toLocal8Bit());
    req.setRawHeader("platform", m_prettyProName.toLocal8Bit());
    req.setRawHeader("architecture", UTILITY::getCpuArchitecture().toLocal8Bit());
    QString data = QString("version=%1").arg(ver);

    if (m_networkAccessManager.networkAccessible() == QNetworkAccessManager::NotAccessible) {
        qWarning()<<"update : QNetworkAccessManager::NotAccessible";
        m_networkAccessManager.setNetworkAccessible(QNetworkAccessManager::Accessible);
    }
    QNetworkReply* reply = m_networkAccessManager.post(req, data.toLocal8Bit());
    if (!reply){
//        if (remindIfNoUpgrade)
//            QMessageBox::warning(this,tr("财政票据客户端综合管理组件"),tr("已安装最新版本，无需更新！"), tr("确定"), nullptr);
        qCritical() << "send update request failure";
        return;
    }
    if (remindIfNoUpgrade)
        reply->setProperty("needRemind", true);
    else
        reply->setProperty("needRemind", false);
    connect(reply, &QNetworkReply::finished, this, &SystemTray::updateFinshed);

}

void SystemTray::updateModule(bool remindIfNoUpgrade)
{
    SysConfigManager mng;
    QString updateUrl = mng.getHTTPConnectionInfo().value(KEY_HTTP_UPDATE_URL).toString();
    int index = updateUrl.lastIndexOf("/");
    updateUrl = updateUrl.left(index+1);
    updateUrl.append("dll");

    Content dllInfo = mng.getSubmodulesInfo();
    QJsonObject obj;
    for (auto iter = dllInfo.constBegin(); iter != dllInfo.constEnd(); iter++){
        QString val = iter.value().toString();
        obj[iter.key()] = val;
    }

    QUrl u = QUrl::fromEncoded(updateUrl.toLocal8Bit());
    QNetworkRequest req(u);
    BusinessService bus;
    if (bus.isUseHttps()) {
        QSslKey key;
        QSslCertificate certificate;
        QList<QSslCertificate> certChain;
        if (!bus.initSslCertFromFile(key, certificate, certChain)) {
            qCritical() << "configure ssl failure";
            return;
        }
        QSslConfiguration config = QSslConfiguration::defaultConfiguration();
        //config.setProtocol(QSsl::TlsV1_2OrLater);
        config.setProtocol(QSsl::AnyProtocol);
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setCaCertificates(certChain);
        config.setLocalCertificate(certificate);
        config.setPrivateKey(key);
        req.setSslConfiguration(config);
    }
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("os", UTILITY::getOs().toLocal8Bit());
    req.setRawHeader("platform", m_prettyProName.toLocal8Bit());
    req.setRawHeader("architecture", UTILITY::getCpuArchitecture().toLocal8Bit());

    if (m_networkAccessManager.networkAccessible() == QNetworkAccessManager::NotAccessible) {
        qWarning()<<"updateModule : QNetworkAccessManager::NotAccessible";
        m_networkAccessManager.setNetworkAccessible(QNetworkAccessManager::Accessible);
    }
    QJsonDocument doc(obj);
    QNetworkReply* reply = m_networkAccessManager.post(req, doc.toJson());
    if (!reply && remindIfNoUpgrade){
        QString appName = getAppName();
        UTILITY::BsMessagebox(nullptr,appName,tr("已安装最新版本，无需更新！"), tr("确定"), nullptr);
        qCritical() << "send update module request failure";
        return;
    }
    if (remindIfNoUpgrade)
        reply->setProperty("needRemind", true);
    else
        reply->setProperty("needRemind", false);
    connect(reply, &QNetworkReply::finished, this, &SystemTray::updateModuleFinshed);
}

void SystemTray::updateFinshed()
{
    QNetworkReply* reply = (QNetworkReply*)sender();
    bool remindIfNoUpgrade = reply->property("needRemind").toBool();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (reply->error() == QNetworkReply::NoError && statusCode == 200) {
        QString name = QString::fromLocal8Bit(reply->rawHeader("app_name"));
        if (name.isEmpty()){
//            if (remindIfNoUpgrade)
//                QMessageBox::warning(this,tr("财政票据客户端综合管理组件"),tr("已安装最新版本，无需更新！"), tr("确定"), nullptr);
            updateModule(remindIfNoUpgrade);
        }
        else{
            QString path = UTILITY::getAbstractPath("./download");
            QDir dir(path);
            if (!dir.exists())
                dir.mkpath(path);

            path = QDir::toNativeSeparators(path);
            path.append(QDir::separator());
            path.append(name);

            QFile qFile(path);
            if (!qFile.open(QIODevice::WriteOnly)){
                qCritical() << "create file failure, path:" << path;
                return;
            }
            qFile.write(reply->readAll());
            qFile.close();

            QProcess process;
            QString cmd = "cp -arf " + path + " " + QDir::tempPath();
            process.start(cmd);
            if(process.waitForFinished()){
                process.close();
            }else {
                process.close();
                qWarning() << cmd << "error";
                return;
            }
            QString appName = getAppName();
            int result = UTILITY::BsMessagebox(nullptr, appName,tr("检测到新版本，需要重启才能更新，是否立即更新？"),tr("是"),tr("否"));
            if (result == QMessageBox::Ok) {
                qInfo() << tr("更新重启");
                //QProcess::startDetached("dpkg", QStringList() << "i" << path);
                path = QDir::tempPath() + "/" + name;
                updatePackage(path, name);
                qApp->exit(0);
            }
//            else {
//                updateModule(remindIfNoUpgrade);
//            }
        }
    }
    else{
        qWarning() << "update failure, QNetworkReply error:" << statusCode << reply->error() << reply->errorString();
        updateModule(remindIfNoUpgrade);
    }
    reply->deleteLater();
}

void SystemTray::updateModuleFinshed()
{
    QString appName = getAppName();
     QNetworkReply* reply = (QNetworkReply*)sender();
    bool remindIfNoUpgrade = reply->property("needRemind").toBool();
    QString path = UTILITY::getAbstractPath("./download");
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (reply->error() == QNetworkReply::NoError && statusCode == 200){
        QString info = reply->rawHeader("UpgradeInfo");
        if (info.isEmpty()){
            //清除旧文件
            QDir dir(path);
            if (dir.exists())
                dir.removeRecursively();
            if (remindIfNoUpgrade) {
                UTILITY::BsMessagebox(nullptr,appName,tr("已安装最新版本，无需更新！"), tr("确定"), nullptr);
            }
        }
        else {
            QDir dir(path);
            if (!dir.exists())
                dir.mkpath(path);
            path = QDir::toNativeSeparators(path);
            path.append(QDir::separator());
            path.append("module_update.zip");
            QFile qFile(path);
            if (!qFile.open(QIODevice::WriteOnly)){
                qCritical() << "create file failure, path:" << path;
                return;
            }
            qFile.resize(0);
            qFile.write(reply->readAll());
            qFile.close();

            int result = UTILITY::BsMessagebox(nullptr,appName,tr("检测到新版本，需要重启才能更新，是否立即更新？"),tr("是"),tr("否"), nullptr);
            if (result == QMessageBox::Ok) {
                qInfo() << tr("更新重启");
                /*
                QString up = UTILITY::getAbstractPath("./upgrade");
                QDir dir1(up);
                if (!dir1.exists())
                    dir1.mkpath(up);
                extract(path, up);
                QString fp = up;
                fp.append(QDir::separator());
                fp.append("upgrade.info");
                QFile qf(fp);
                if (qf.open(QIODevice::WriteOnly)){
                    qf.write(info.toLocal8Bit());
                    qf.close();
                }*/
                updateModuleStatus ret;
                ret = copyModuleFile(info);
                if (ret != UPDATE_MODULE_SUCCESS) {
                    qWarning() << "copyModuleFile error(" << ret << ")!";
                    return;
                }
                restartApp();
//                QProcess::startDetached(path, QStringList());
//                extract(path, dst);
//                qApp->exit(-4);
            }
        }
    }
    else{
        qWarning() << "update module failure, QNetworkReply error:" << statusCode << reply->error() << reply->errorString();
        //清除旧文件
        QDir dir(path);
        if (dir.exists())
            dir.removeRecursively();
        if (remindIfNoUpgrade)
            UTILITY::BsMessagebox(nullptr,appName,tr("已安装最新版本，无需更新！"), tr("确定"), nullptr);
    }
    reply->deleteLater();
}

void SystemTray::startUpdateTimer(bool isFirst)
{
    SysConfigManager mng;
    Content appInfo = mng.getAppInfo();
    int interval = appInfo.value(KEY_APP_UPDATE_INTERVAL).toString().toInt();
    if (interval == 0) {
        interval = 8;
    }

    //启动后先检测一次
    if (isFirst)
        QTimer::singleShot(3000, this, &SystemTray::updateTimer);
    //定时检测更新
    if (m_updateTimer){
        if (m_updateTimer->isActive()){
            m_updateTimer->stop();
            disconnect(m_updateTimer, &QTimer::timeout, this, &SystemTray::updateTimer);
        }
        connect(m_updateTimer, &QTimer::timeout, this, &SystemTray::updateTimer);
        m_updateTimer->start(interval * 1000 * 60 * 60);
    }
}

static bool runCopyCmd(const QString& cmd)
{
    QStringList options;
    options << "-c" << cmd;
    QProcess process;
    process.start("/bin/bash", options);
    if(process.waitForFinished()) {
        process.close();
        return true;
    } else {
        process.close();
        qWarning() << cmd << "error";
        return false;
    }
}

void SystemTray::updatePackage(const QString& packagePath, const QString& packageName)
{
    QString upgradeCmd;
    BusinessService businessService;

    if(m_prettyProNameFlag == PRETTY_PRODUCT_NAME_UOS) {
        upgradeCmd = businessService.readIniSys("APP", KEY_APP_UPGRADE_CMD_UOS, "");
    } else if(m_prettyProNameFlag == PRETTY_PRODUCT_NAME_KYLIN) {
        upgradeCmd = businessService.readIniSys("APP", KEY_APP_UPGRADE_CMD_KYLIN, "");
    } else if(m_prettyProNameFlag == PRETTY_PRODUCT_NAME_NEOKYLIN) {
        upgradeCmd = businessService.readIniSys("APP", KEY_APP_UPGRADE_CMD_NEOKYLIN, "");
    } else{
        upgradeCmd = "";
    }
    if(upgradeCmd.isEmpty()) {
        return;
    }

    if (m_prettyProNameFlag == PRETTY_PRODUCT_NAME_KYLIN) {
        QString packageDirPath = QDir::tempPath()+"/bosssoft_upgrade";
        QDir dir(packageDirPath);
        if (!dir.exists()) {
            dir.mkpath(packageDirPath);
        }
        QString cpUpgradePkgCmd = "cp -arf " + packagePath + " " + packageDirPath;
        runCopyCmd(cpUpgradePkgCmd);
        QString kylin_installer_path = "/usr/share/kylin-installer/*";
        QString cpUpgradeToolsCmd = "cp -arf " + kylin_installer_path + " " + packageDirPath;
        runCopyCmd(cpUpgradeToolsCmd);
        QString pkgPath = packageDirPath + "/" + packageName;
        upgradeCmd.append(" ");
        upgradeCmd.append(pkgPath);
    } else {
        upgradeCmd.append(" ");
        upgradeCmd.append(packagePath);
    }

    QProcess process;
    process.startDetached(upgradeCmd);

    return;
}

void SystemTray::getPrettyProInfo()
{
   QString prettyProName = QSysInfo::prettyProductName();
   BusinessService businessService;

   if(prettyProName.startsWith(UOS_PRETTY_PRO_NAME)) {
       m_prettyProNameFlag = PRETTY_PRODUCT_NAME_UOS;
       m_prettyProName = "uos";
   } else if(prettyProName.startsWith(KYLIN_PRETTY_PRO_NAME)) {
       m_prettyProNameFlag = PRETTY_PRODUCT_NAME_KYLIN;
       m_prettyProName = "kylin";
   } else if(prettyProName.startsWith(NEOKYLIN_PRETTY_PRO_NAME)) {
       m_prettyProNameFlag = PRETTY_PRODUCT_NAME_NEOKYLIN;
       m_prettyProName = "neokylin";
   } else {
       m_prettyProNameFlag = PRETTY_PRODUCT_NAME_OTHER;
       m_prettyProName = "";
   }

   return;
}

static QJsonObject getJsonObjFromString(const QString& jsonString)
{
    QJsonObject obj;
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toLocal8Bit(), &error);
    if(!doc.isNull() && error.error == QJsonParseError::NoError) {
        if(doc.isObject()) {
            obj = doc.object();
        }
    }
    return obj;
}

updateModuleStatus SystemTray::copyModuleFile(const QString& info)
{
    QString currentPath = QCoreApplication::applicationDirPath();
    QString dirPath = currentPath + "/download";

    if(extract(dirPath + "/module_update.zip", dirPath) != 0) {
        qWarning() << "unzip " + dirPath + "/module_update.zip fail!";
        return UPDATE_MODULE_UNZIP_ERROR;
    }

    QJsonObject jsonInfo = getJsonObjFromString(info);
    QJsonObject::const_iterator iter = jsonInfo.constBegin();

    QDir dir(dirPath + "/upgrade");
    if(!dir.exists()) {
        qWarning() << "dir " + dirPath + "/upgrade" + " does not exist";
        return UPDATE_MODULE_FILE_NOT_EXIST;
    }
    QStringList filters;
    filters << QString("*.%1").arg("zip");
    dir.setNameFilters(filters);
    QFileInfoList listTmp = dir.entryInfoList();

    while (iter != jsonInfo.constEnd()) {
        QString node = iter.key();
        QFileInfoList list;
        foreach(QFileInfo item, listTmp) {
            if (item.completeBaseName().startsWith(node)) {
                list.append(item);
            }
        }
        if(list.isEmpty()) {
            qWarning() << "no files with zip suffix were found(" << node << ")";
            return UPDATE_MODULE_FILE_NOT_EXIST;
        }

        if(extract(list.at(0).absoluteFilePath(), dirPath + "/upgrade") != 0) {
            qWarning() << "unzip " + list.at(0).absoluteFilePath() + " fail!";
            return UPDATE_MODULE_UNZIP_ERROR;
        }

        QString cmd = "cp -arf ";
        QString iniPath = currentPath + "/conf/dll.ini";
        QSettings *iniFile = new QSettings(iniPath, QSettings::IniFormat);
        QString updateRelativePath = iniFile->value("/MODULE-UPDATE-PATH/" + node).toString();
        if (updateRelativePath.isEmpty()) {
            if(node == "ca") {
                updateRelativePath.append(DEFAULT_CA_UPDATE_PATH);
            } else if(node == "camera") {
                updateRelativePath.append(DEFAULT_CAMERA_UPDATE_PATH);
            } else if(node == "pd") {
                updateRelativePath.append(DEFAULT_PD_UPDATE_PATH);
            } else if(node == "pos") {
                updateRelativePath.append(DEFAULT_POS_UPDATE_PATH);
            } else if(node == "print") {
                updateRelativePath.append(DEFAULT_PRINT_UPDATE_PATH);
            } else if(node == "upload") {
                updateRelativePath.append(DEFAULT_UPLOAD_UPDATE_PATH);
            } else if(node == "NontaxAgencyActuator") {
                updateRelativePath.append(DEFAULT_NONTAXAGENCYACTUATOR_UPDATE_PATH);
            } else if(node == "smart-card") {
                updateRelativePath.append(DEFAULT_SMART_CARD_UPDATE_PATH);
            } else if(node == "qrcodepay") {
                updateRelativePath.append(DEFAULT_QRCODEPAY_UPDATE_PATH);
            } else if(node == "boss-report") {
                updateRelativePath.append(DEFAULT_BOSS_REPORT_UPDATE_PATH);
            } else {
                qWarning() << "undefined node: " + node;
                return UPDATE_MODULE_UPGRADE_INFO_ERROR;
            }
        }
        delete iniFile;

        QString dstPath = currentPath + "/" + updateRelativePath;
        cmd.append(dirPath + "/upgrade/" + node + "/*" + " " + dstPath);
        QStringList options;
        options << "-c" << cmd;
        QProcess process;
        process.start("/bin/bash", options);
        if(process.waitForFinished()) {
            process.close();
        } else {
            process.close();
            qWarning() << cmd << "error";
            return UPDATE_MODULE_CMD_ERROR;
        }

        iniPath = currentPath + "/conf/sys.ini";
        QString iniKey = "/DLL/" + node;
        QString iniValue = "v" + iter.value().toString();
        QSettings *configIniWrite = new QSettings(iniPath, QSettings::IniFormat);
        configIniWrite->setValue(iniKey, iniValue);
        delete configIniWrite;

        iter++;
    }

    QDir dir1(dirPath + "/upgrade");
    if (dir1.exists()) {
        dir1.removeRecursively();
    }

    return UPDATE_MODULE_SUCCESS;
}

