#include "businessservice.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QSharedPointer>
#include <QLibrary>
#include <QFile>
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include "submoduleconfigmanager.h"
#include "sysconfigmanager.h"
#include "configmanager.h"
#include "inifilehandler.h"
#include <QCoreApplication>
#include <QDir>

#define DLL_MEM_SIZE 1024*1024

BusinessService::BusinessService()
{

}

QByteArray BusinessService::serviceEntry(QString appFileDir, QByteArray payload, QString module, QString func)
{
    BusinessService pBusinessService;
    QString config, data, dllPath;
    char *cRes;
    int ret;

    config = pBusinessService.buildCommonConfig(payload, appFileDir);
    if (config == BSERROR_RESULT) {
        return (pBusinessService.setJsonString("2001", QObject::tr("config数据异常，不符合json格式")));
    }
    data = pBusinessService.buildCommonData(payload);
    if ((data == BSERROR_RESULT) || (data == BSERROR_VALUE_NIL)) {
        return (pBusinessService.setJsonString("2001", QObject::tr("data数据异常，不符合json格式")));
    }
    dllPath =  QCoreApplication::applicationDirPath() + "/" + pBusinessService.readIniDll("MODULE-DLLPATH", module, "");
    if (dllPath.isEmpty()) {
        return (pBusinessService.setJsonString("2003", QObject::tr("未找到指定的DLL文件")));
    }
/*
    baConfig = config.toLatin1();
    cConfig = baConfig.data();
    baData = data.toLatin1();
    cData = baData.data();
    baFunc = func.toLatin1();
    cFunc = baFunc.data();
*/
    QLibrary lib(dllPath);
      if (lib.load()) {
        typedef int (*callLibFunc)(const char*, const char*, char*, int);
        callLibFunc callFunc = (callLibFunc) lib.resolve(qPrintable(func));
        if (callFunc) {
            cRes = new char[DLL_MEM_SIZE];
            if (cRes == nullptr) {
                qWarning()<<"Request memory space error";
                return (pBusinessService.setJsonString("2005", QObject::tr("动态库使用空间，申请失败")));
            }
            memset(cRes, 0, DLL_MEM_SIZE);
            ret = callFunc(qPrintable(data), qPrintable(config), cRes, DLL_MEM_SIZE);
            if (ret == 0) {
                //qDebug()<<"cRes : "<<cRes;
                QByteArray baRes;
                baRes = QByteArray(cRes);

                //qInfo()<<"dll return :"<<baRes;
                delete [] cRes;
                return baRes;
            } else {
                delete [] cRes;
                QString exception;
                exception = "捕获到异常："+func+QString::number(ret);
                qWarning()<<exception;
                return (pBusinessService.setJsonString("2001", QObject::tr(exception.toLatin1())));
            }
        } else {
            return (pBusinessService.setJsonString("2002", QObject::tr("在DLL中未找到指定的方法")));
        }
    } else {
        qWarning()<<"load failure, reason: "<<lib.errorString();
        return (pBusinessService.setJsonString("2003", QObject::tr("未找到指定的DLL文件")));
    }
}

QByteArray BusinessService::doBase64Decode(QByteArray src, int decMode)
{
    return QByteArray::fromBase64(src);
}

QByteArray BusinessService::doBase64Encode(QByteArray src, int encMode)
{
    return src.toBase64();
}

QString BusinessService::buildCommonConfig(QByteArray payload, QString appFileDir)
{
    QJsonParseError jsonError;
    QJsonValue appID;

    QJsonDocument document = QJsonDocument::fromJson(payload, &jsonError);
    if (document.isNull() || (jsonError.error != QJsonParseError::NoError)) {
        return BSERROR_RESULT;
    }
    if (!document.isObject()) {
        return BSERROR_RESULT;
    }

    QJsonObject obj = document.object();
    if (!obj.contains(KEY_CONFIG)) {
        return BSERROR_RESULT;
    }

    QJsonValue config = obj.value(KEY_CONFIG);
    if (!config.isObject()) {
        return BSERROR_RESULT;
    }

    obj = config.toObject();
    if (obj.contains(KEY_APP_ID)) {
        appID = obj.value(KEY_APP_ID);
    } else {
        return BSERROR_RESULT;
    }

    if (appID.isNull()) {
        return BSERROR_RESULT;
    }
    QJsonObject buildConfig;
    buildConfig.insert(KEY_APP_ID, appID);
    buildConfig.insert(KEY_APP_FILE_DIR, appFileDir);
    document.setObject(buildConfig);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    return strJson;
}

QString BusinessService::buildCommonData(QByteArray payload)
{
    QJsonParseError jsonError;
    QJsonArray dataArray;

    QJsonDocument document = QJsonDocument::fromJson(payload, &jsonError);
    if (document.isNull() || (jsonError.error != QJsonParseError::NoError)) {
        return BSERROR_RESULT;
    }
    if (!document.isObject()) {
        return BSERROR_VALUE_NIL;
    }

    QJsonObject obj = document.object();
    if (!obj.contains(KEY_DATA)) {
        return BSERROR_VALUE_NIL;
    }

    QJsonValue data = obj.value(KEY_DATA);
    if (!data.isArray()) {
        return BSERROR_VALUE_NIL;
    }
    dataArray = data.toArray();
    if (dataArray.size() == 0) {
        return BSERROR_VALUE_NIL;
    }
    data = dataArray.at(0);
    if ((!data.isObject()) || data.isNull()) {
        return BSERROR_VALUE_NIL;
    }
/*
    QJsonArray newDataArray;
    newDataArray.append(data);
    QJsonObject buildData;
    buildData.insert(KEY_DATA, QJsonValue(newDataArray));
    document.setObject(buildData);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
*/
    document.setObject(data.toObject());
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    return strJson;
}

QByteArray BusinessService::setJsonString(QString retCode, QString retMsg)
{
    QJsonObject json;
    json.insert(KEY_RET_CODE, retCode);
    json.insert(KEY_RET_MSG, retMsg);

    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);

    return  byteArray;
}

QString BusinessService::readIniDll(QString module, QString key, QString defaultValue)
{
/* for rookie */
//    SubmoduleConfigManager tmpSubConfManager;
//    Content maps;
//    if (Module == "MODULE-DLLPATH") {
//        maps = tmpSubConfManager.getModulePaths();
//    } else if (Module == "MODULE-LANG") {
//        maps = tmpSubConfManager.getModuleLang();
//    } else {
//        return defaultValue;
//    }

//    if(maps.contains(key)) {
//        return maps[key].toString();
//    } else {
//        return defaultValue;
//    }

/* for senior */
    if((module != "MODULE-DLLPATH") &&  (module != "MODULE-LANG") )
        return defaultValue;

    SubmoduleConfigManager tmpSubConfManager;
    Content &maps = (module == "MODULE-DLLPATH")
                    ?tmpSubConfManager.getModulePaths()
                    :tmpSubConfManager.getModuleLang();

    return (maps.contains(key)?maps[key].toString():defaultValue);
}

QString BusinessService::readIniSys(QString module, QString key, QString defaultValue)
{
    SysConfigManager tmpSysConfManager;
    Content maps;
    if (module == "HTTP") {
        maps = tmpSysConfManager.getHTTPConnectionInfo();
    } else if (module == "APP") {
        maps = tmpSysConfManager.getAppInfo();
    } else if (module == "DLL") {
        maps = tmpSysConfManager.getSubmodulesInfo();
    } else {
        return defaultValue;
    }

    if(maps.contains(key)) {
        return maps[key].toString();
    } else {
        return defaultValue;
    }
}

void BusinessService::initCommuParams(void)
{
    QString serverIP, serverPort;
    serverIP = readIniSys("HTTP", KEY_SERVER_IP, DEFAULT_SERVER_IP);
    serverPort = readIniSys("HTTP", KEY_SERVER_PORT, DEFAULT_HTTP_SERVER_PORT);

    QString _filePath =ConfigManager::getInstance()->getSysConfigManager()->filePath();
    QSharedPointer<IniFileHandler> tmpFileHandler(new IniFileHandler(_filePath));
    if (tmpFileHandler->exist()) {
        tmpFileHandler->setValue("/listener/host", serverIP);
        tmpFileHandler->setValue("/listener/port", serverPort);
        tmpFileHandler->setValue("/listenerhttps/host", serverIP);
        tmpFileHandler->setValue("/listenerhttps/port", QString ::number((serverPort.toInt()-1)));
    } else {
        return;
    }
}

bool BusinessService::isAgency3Intfc(const QString &payload)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(payload.toLocal8Bit(), &error);
    if (error.error != QJsonParseError::NoError){
       return false;
    }
    QJsonObject obj = doc.object();
    if (obj.contains(KEY_CONFIG)){
        QJsonObject opt = obj[KEY_CONFIG].toObject();
        if (opt.contains("agency3intfc"))
            return opt["agency3intfc"].toBool();
    }
    return false;
}

QByteArray BusinessService::ReadScriptText()
{
    QByteArray script;
    QFile file(":/Script/Resources/json2.txt");
    if (!file.open(QIODevice::ReadOnly))
        return script;
    script = file.readAll();
    file.close();
    return script;
}

QByteArray BusinessService::BuildResponseScript(const QString &id, const QByteArray &textCode, const QByteArray &res)
{
    QJsonObject obj;
    obj["id"] = id;

    QJsonDocument doc = QJsonDocument::fromJson(res);
    obj["ret"] = doc.object();

    QJsonDocument doc1(obj);
    QString response = QString("<script>%1var data=%2;window.parent.postMessage(JSON.stringify(data), \"*\")</script>").arg(QString(textCode)).arg(QString(doc1.toJson(QJsonDocument::Compact)));
    return response.toLocal8Bit();
}

void BusinessService::writeIniSys(QString module, QString key, QString value)
{
    QString _filePath =ConfigManager::getInstance()->getSysConfigManager()->filePath();
    QSharedPointer<IniFileHandler> tmpFileHandler(new IniFileHandler(_filePath));
    if (tmpFileHandler->exist()) {
        QString keyComp = QString("/%1/%2").arg(module).arg(key);
        //qDebug()<<"key :"<<keyComp;
        tmpFileHandler->setValue(keyComp, value);
    }
}

void BusinessService::loadRelyDll(void)
{
    QString printRelyDllPath;
#if 0
    QDir dir(QCoreApplication::applicationDirPath());
    bool bSucess = dir.cdUp();
    if (bSucess) {
        printRelyDllPath = dir.path() + PRINT_RELY_DLL;
        QLibrary libRely(printRelyDllPath);
        libRely.load();
    }
#endif
    //QString printRelyDllPath = QCoreApplication::applicationDirPath() + PRINT_RELY_DLL;
    printRelyDllPath = QCoreApplication::applicationDirPath() + PRINT_RELY_DLL;
    QLibrary libRely(printRelyDllPath);
    libRely.load();

    if (!libRely.load()) {
        //return (pBusinessService.setJsonString("2003", QObject::tr("print依赖库加载失败")));
        qWarning()<<"print依赖库加载失败 : "<<libRely.errorString();
    }

}

void BusinessService::initService(void)
{
    loadRelyDll();
}

void BusinessService::buildTemplateJsonFile(void)
{
    QJsonObject buildObj;
    buildObj.insert("do_print", "reciept-demo-template.lrxml");
    buildObj.insert("do_priview", "reciept-demo-template.lrxml");
    QJsonDocument document;
    document.setObject(buildObj);
    QString path = QCoreApplication::applicationDirPath() + "/data/json/template.json";
    QFile file(path);
    file.open(QIODevice::ReadWrite);
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
}

bool BusinessService::initSslCertFromFile(QSslKey &key, QSslCertificate &certificate, QList<QSslCertificate> &certChain)
{
    QString certPath, certPsw;

    certPath = readIniSys(SYS_INI_HTTP_MODULE, KEY_SYS_CERT_PATH, "");
    certPsw = readIniSys(SYS_INI_HTTP_MODULE, KEY_SYS_CERT_PSW, "");
    if (certPath.isEmpty() || certPsw.isEmpty()) {
        return false;
    }

    QFile pfxFile(QCoreApplication::applicationDirPath()+ certPath);
    if (!pfxFile.open(QFile::ReadOnly)) {
        return false;
    }

    bool imported = QSslCertificate::importPkcs12(&pfxFile, &key, &certificate, &certChain, certPsw.toLatin1());
    if (imported) {
        return true;
    } else {
        return false;
    }
}

bool BusinessService::isUseHttps(void)
{
    QString sysIniFile = QCoreApplication::applicationDirPath() + SYS_INI_PATH;
    QSettings *pIni = new QSettings(sysIniFile, QSettings::IniFormat);
    bool result = false;
    if ((pIni->value("/HTTP/server.openssl").toString()) == "1") {
        result = true;
    }
    delete pIni;
    return result;
}
