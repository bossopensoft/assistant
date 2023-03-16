#include "caconfigmanager.h"
#include "inifilehandler.h"
#include "configmanager.h"

#include <QMap>
#include <QTextStream>
#include <QDebug>

#include <algorithm>
#include <istream>
#include <string>
#include <map>


CAConfigManager::CAConfigManager()
                : CAConfigManager(*ConfigManager::getInstance()->getCAConfigManager())
{

}

CAConfigManager::CAConfigManager(const CAConfigManager &other)
               : mFileHandler(other.mFileHandler)
{
    load();
}

CAConfigManager::CAConfigManager(IniFileHandler* fileHandler)
                : mFileHandler(fileHandler)
{
    load();
}

Content &CAConfigManager::getClass()
{
    return mClassContent;
}

QString CAConfigManager::filePath() const
{
   return mFileHandler->filePath();
}

bool CAConfigManager::save()
{
    return true;
}

bool CAConfigManager::load()
{
    if(!mFileHandler->exist())
    {
        qWarning()<<"file path "<<mFileHandler->filePath()<<" was not exist!";
        return false;
    }

    foreach(QString _k, mFileHandler->childKeys("CLASS")) {
            mClassContent[_k] = mFileHandler->value("CLASS/"+_k);
    }

    return true;
}

