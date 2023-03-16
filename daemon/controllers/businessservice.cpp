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
#include <QCoreApplication>
#include <QDir>

#define DLL_MEM_SIZE 1024*1024

BusinessService::BusinessService()
{

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


QString BusinessService::readIniSys(QString module, QString key, QString defaultValue)
{

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
