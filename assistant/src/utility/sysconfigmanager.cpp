#include "sysconfigmanager.h"
#include "inifilehandler.h"
#include "configmanager.h"

#include <QDebug>

SysConfigManager::SysConfigManager()
                : SysConfigManager(*ConfigManager::getInstance()->getSysConfigManager())
{
    load();
}

SysConfigManager::SysConfigManager(const SysConfigManager &other)
                : mFileHandler(other.mFileHandler)
{

}

QString SysConfigManager::filePath() const
{
    return mFileHandler->filePath();
}

SysConfigManager::SysConfigManager(IniFileHandler *handler)
                : mFileHandler(handler)
{

}


Content &SysConfigManager::getHTTPConnectionInfo()
{
    return mHttpConfig;
}

Content &SysConfigManager::getAppInfo()
{
    return mAppInfoConfig;
}

Content &SysConfigManager::getSubmodulesInfo()
{
    return mDllVersionInfo;
}

bool SysConfigManager::save()
{
    return true;
}

bool SysConfigManager::load()
{

    if(!mFileHandler->exist())
    {
        qWarning()<<"file path "<<mFileHandler->filePath()<<" was not exist!";
        return false;
    }

    foreach(QString _k, mFileHandler->childKeys("HTTP")) {
            mHttpConfig[_k] = mFileHandler->value("HTTP/"+_k);
    }

    foreach(QString _k, mFileHandler->childKeys("APP")) {
            QString  _path="APP/"+_k;
            //if(!mFileHandler->contains(_path)){
                //qWarning()<<"key:"<<_path<<"was not exsit!";
                //continue;
            //}
            mAppInfoConfig[_k] = mFileHandler->value(_path);
    }

    foreach(QString _k, mFileHandler->childKeys("DLL")) {
            //QVariant _v= mFileHandler->value(_k);
            //qDebug()<<"_k "<<_k<<" v:"<<_v.toString();
            mDllVersionInfo[_k] = mFileHandler->value("DLL/"+_k);
    }
    return true;
}


