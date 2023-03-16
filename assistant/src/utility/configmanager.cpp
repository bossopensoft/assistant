#include <QCoreApplication>
#include <array>
#include <QMessageBox>
#include <QDebug>

#include "configmanager.h"
#include "inifilehandler.h"
#include "utility.h"

ConfigManager* ConfigManager::mInstance = nullptr;

enum {
    SYS_CONFIG_FILE_PATH,
    SUBMODULES_CONFIG_PATH,
    CA_CONFIG_FILE_PATH,
};


static inline
QVector<IniFileHandler*>
__mapper_initializor()
{

    const std::array<QString, 3> _paths=
    {
        "sys.ini",
        "dll.ini",
        "ca.ini",
    };

    auto __create_handler=[](const QString& name)->IniFileHandler*
    {
        const QString path = UTILITY::getAbstractPath("conf/"+name);
        return (new IniFileHandler(path));
    };

    QVector<IniFileHandler*> __list;

    int _num=_paths.size();

    while(_num--){
        __list.append(__create_handler(_paths[_num]));
    }

    return __list;
}

ConfigManager::ConfigManager()
             : mFileHandlers(__mapper_initializor())
             , mHttpConfigPath(UTILITY::getAbstractPath("conf/sys.ini"))
             , mCAConfigManager(new CAConfigManager(mFileHandlers[SYS_CONFIG_FILE_PATH]))
             , mSubModuleConfigManager(new SubmoduleConfigManager(mFileHandlers[SUBMODULES_CONFIG_PATH]))
             , mSysConfigManager(new SysConfigManager(mFileHandlers[CA_CONFIG_FILE_PATH]))
{

}

bool ConfigManager::load()
{
    /// for test purpese.
    for(IniFileHandler* handler: mFileHandlers)
    {
        if(handler->exist()) continue;

        const char* __notice="配置文件%1:不存在,请在确认后再启动当前程序！";
        qWarning()<<"ini file "<<handler->filePath()<<" not exist! please take a look before running this application!";
        QMessageBox::critical(nullptr, QObject::tr("配置文件不存在警告"), QString(__notice).arg(handler->filePath()));
        return false;
    }

    return true;
}

QString ConfigManager::httpConnectionConfigFilePath() const
{
    return mHttpConfigPath;
}



QString ConfigManager::errorReason() const
{
    return mErrorReason;
}

CAConfigManager* ConfigManager::getCAConfigManager()
{
    return mCAConfigManager;
}

SubmoduleConfigManager *ConfigManager::getSubModulesConfigManager()
{
    return mSubModuleConfigManager;
}

SysConfigManager *ConfigManager::getSysConfigManager()
{
    return mSysConfigManager;
}

ConfigManager *ConfigManager::getInstance()
{
    if(mInstance == nullptr) mInstance =new ConfigManager();

    return mInstance;
}
