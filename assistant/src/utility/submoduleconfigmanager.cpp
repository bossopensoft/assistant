#include "submoduleconfigmanager.h"
#include "configmanager.h"
#include "inifilehandler.h"

#include <QDebug>

SubmoduleConfigManager::SubmoduleConfigManager(IniFileHandler *fileHandler)
                      : mFileHandler(fileHandler)
{
    load();
}

SubmoduleConfigManager::SubmoduleConfigManager()
                      : SubmoduleConfigManager(*ConfigManager::getInstance()->getSubModulesConfigManager())
{

}

SubmoduleConfigManager::SubmoduleConfigManager(const SubmoduleConfigManager &other)
                      : SubmoduleConfigManager(other.mFileHandler)
{

}

QString SubmoduleConfigManager::filePath() const
{
    return mFileHandler->filePath();
}


Content &SubmoduleConfigManager::getModulePaths()
{
    return mModulePathsContent;
}

Content &SubmoduleConfigManager::getModuleLang()
{
    return mModuleLangContent;
}

bool SubmoduleConfigManager::save()
{
    // TODO implement later.
    return true;
}

bool SubmoduleConfigManager::load()
{
    if(!mFileHandler->exist())
    {
        qWarning()<<"file path "<<mFileHandler->filePath()<<" was not exist!";
        return false;
    }

    foreach(QString _k, mFileHandler->childKeys("MODULE-DLLPATH")) {
            QVariant _v= mFileHandler->value("MODULE-DLLPATH/"+_k);
            //qDebug()<<"_k "<<_k<<" v:"<<_v.toString();
            mModulePathsContent[_k] = mFileHandler->value("MODULE-DLLPATH/"+_k);
    }

    foreach(QString _k, mFileHandler->childKeys("MODULE-LANG")) {
            QVariant _v= mFileHandler->value("MODULE-LANG/"+_k);
            //qDebug()<<"_k "<<_k<<" v:"<<_v.toString();
            mModuleLangContent[_k] = mFileHandler->value("MODULE-LANG/"+_k);
    }

    return true;
}
