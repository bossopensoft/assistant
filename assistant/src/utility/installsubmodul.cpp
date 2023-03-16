#include "installsubmodul.h"
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtDebug>
#include "../utility/utility.h"
#include "../utility/ziputils.h"
#include "../controllers/businessservice.h"

InstallSubmoduleThread::InstallSubmoduleThread(QObject *parent) : QObject(parent)
{
    connect(this, &InstallSubmoduleThread::installSubmodule, this, &InstallSubmoduleThread::installModule);
}

void InstallSubmoduleThread::install()
{
    emit installSubmodule();
}

void InstallSubmoduleThread::installModule()
{
        QString path = UTILITY::getAbstractPath("./upgrade");
        QDir dir(path);
        if (!dir.exists()){
            emit startUpdateTimer(true);
            return;
        }
        QString fp = path;
        fp.append(QDir::separator());
        fp.append("upgrade.info");
        if (!QFile::exists(fp)){
            dir.removeRecursively();
            emit startUpdateTimer(true);
            return;
        }
        QFile qf(fp);
        if (qf.open(QIODevice::ReadOnly)){
            QByteArray info = qf.readAll();
            qf.close();
            QJsonDocument doc = QJsonDocument::fromJson(info);
            QJsonObject obj = doc.object();
            for (auto iter = obj.begin(); iter != obj.end(); iter++){
                QString key = iter.key();
                QString val = iter.value().toString();
                QString zipFile = QString("%1%2%3.zip").arg(path).arg(QDir::separator()).arg(key);
                if (!QFile::exists(zipFile))
                    continue;
                QString subDIr = QString("%1%2%3").arg(path).arg(QDir::separator()).arg(key);
                if (extract(zipFile, subDIr) != 0){
                    qCritical() << "extract zip fail:" << zipFile;
                    continue;
                }
                QString shFile = QString("/bin/bash %1%2update.sh").arg(subDIr).arg(QDir::separator());
                QProcess p;
                p.start(shFile);
                if (p.waitForFinished()){
                    BusinessService bus;
                    bus.writeIniSys("DLL", key, "v" + val);
                }
            }
        }
        dir.removeRecursively();
        emit startUpdateTimer(true);
}
