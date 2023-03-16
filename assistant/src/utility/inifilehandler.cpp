#include "inifilehandler.h"
#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>

#define __LOGGIN_ERROR(...) qCritical(__VA_ARGS__)

IniFileHandler::IniFileHandler(const QString path)
            : mFilePath(path)
            , mIniFileHandler(new QSettings(path, QSettings::IniFormat))
{

}

IniFileHandler::~IniFileHandler()
{
   if(mIniFileHandler) mIniFileHandler->deleteLater();

   mIniFileHandler = nullptr;
}

bool IniFileHandler::contains(const QString &key)
{
   return mIniFileHandler->contains(key);
}

void IniFileHandler::setValue(const QString &key, const QVariant &value)
{
    mIniFileHandler->setValue(key, value);
}

QVariant IniFileHandler::value(const QString &key, const QVariant &defaultValue)const
{
    return mIniFileHandler->value(key, defaultValue);
}

bool IniFileHandler::exist()
{
    return QFile::exists(mFilePath);
}

QStringList IniFileHandler::childKeys(const QString &section) const
{
    mIniFileHandler->beginGroup(section);
    QStringList _keys= mIniFileHandler->childKeys();
    mIniFileHandler->endGroup();

    QStringList _dupList;
    foreach (QString _key, _keys)
    {
        if(_key.startsWith("#")) continue; // to ignore it.
        _dupList.append(_key);
    }
    return _dupList;
}
