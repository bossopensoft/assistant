/***************************************************************************
 *   This file is part of the Bosssoft Assistant Client Project                          *
 *   Copyright (C) 2023 by Bin Yan                                                                *
 *   yanbin@bosssoft.com.cn                                                                        *
 *                                                                                                                   *
 **                   GNU General Public License Usage                                      **
 *                                                                                                                  *
 *   This library is free software: you can redistribute it and/or modify        *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or          *
 *   (at your option) any later version.                                                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                                                                   *
 **                  GNU Lesser General Public License                                      **
 *                                                                                                                   *
 *   This library is free software: you can redistribute it and/or modify         *
 *   it under the terms of the GNU Lesser General Public License as             *
 *   published by the Free Software Foundation, either version 3 of the       *
 *   License, or (at your option) any later version.                                         *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with this library.                                                                *
 *   If not, see <http://www.gnu.org/licenses/>.                                            *
 *                                                                                                                   *
 *   This library is distributed in the hope that it will be useful,                     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the   *
 *   GNU General Public License for more details.                                          *
 ****************************************************************************/

#ifndef LOGGERUTILS_H
#define LOGGERUTILS_H
#include <QObject>
#include <QtDebug>
#include <QFileSystemWatcher>
#include <QTime>
#include "utility.h"
#include "../tinylogger/tinylogger.h"

class LoggerUtils : public QObject
{
    Q_OBJECT
public:
    LoggerUtils()
        :mSysPath(UTILITY::getAbstractPath("conf/tinylogger.ini")),
        mLogger(nullptr){
        connect(&mFsWatcher, &QFileSystemWatcher::fileChanged, this, &LoggerUtils::fileChanged);
        if (!mFsWatcher.addPath(mSysPath))
            qWarning() << "monitor file fail";
    }

    ~LoggerUtils(){
        if (mLogger){
            delete mLogger;
            mLogger = nullptr;
        }
    }

    TinyLogger* getLogger(){
        return mLogger;
    }

    bool initLog(){
        if (!mLogger){
            mLogger = new TinyLogger();
            if (!mLogger){
                qWarning() << "create logger class fail";
                return false;
            }
        }
        QString logDir, logName, logExtern;
        logDir = UTILITY::getConfigFromIni(mSysPath, "TinyLogger/workspace" , UTILITY::getAbstractPath("log/assistant")).toString();
        logName = UTILITY::getConfigFromIni(mSysPath, "TinyLogger/baseFileName" , "bosssoft-assistant").toString();
        logExtern = UTILITY::getConfigFromIni(mSysPath, "TinyLogger/fileNameExtension" , ".log").toString();
        mOldLevel = UTILITY::getConfigFromIni(mSysPath, "TinyLogger/level" , "info").toString().toLower();
        size_t maxSize = UTILITY::getConfigFromIni(mSysPath, "TinyLogger/maxFileSize" , "10").toUInt() * 1024 * 1024;
        size_t maxNum = UTILITY::getConfigFromIni(mSysPath, "TinyLogger/maxFileNum" , "3").toUInt();

        mLogger->setLoggerConfiguration(TinyLogger::Configuration { logDir.toStdString().c_str(),
                                                                    logName.toStdString().c_str(),
                                                                    logExtern.toStdString().c_str(),
                                                                    maxSize,
                                                                    maxNum});
        mLogger->setAvaliableLV(leverConverter(mOldLevel));
        if (!mLogger->initialization()){
            qWarning() << "logger initialization fail";
            return false;
        }
        return true;
    }

    quint32 MsgTypeConverter(QtMsgType type)
    {
        if (type == QtMsgType::QtDebugMsg)
            return 0x00000001;
        else if (type == QtMsgType::QtInfoMsg)
            return 0x00000002;
        else if (type == QtMsgType::QtWarningMsg)
            return 0x00000004;
        else if (type == QtMsgType::QtCriticalMsg || type == QtMsgType::QtSystemMsg)
            return 0x00000008;
        else if (type == QtMsgType::QtFatalMsg)
            return 0x00000010;
        else
            return 0x00000002;
    }
signals:
    void monitor(const QString &path);
public slots:
    void fileChanged(const QString &path){
        if (path.compare(mSysPath) == 0 && mLogger){
            QString level = UTILITY::getConfigFromIni(mSysPath, "TinyLogger/level" , "").toString().toLower();
            if (level.isEmpty() || mOldLevel.compare(level) == 0)
                return;
            qInfo() << "sys.ini had changed, level=%s" << level.toStdString().c_str() << "time=%s" << QTime::currentTime().toString().toStdString().c_str();
            mOldLevel = level;
            mLogger->setAvaliableLV(leverConverter(level));
            if (QFile::exists(mSysPath)) {
                mFsWatcher.removePath(mSysPath);
                if (!mFsWatcher.addPath(mSysPath)){
                    qWarning() << "monitor file fail";
                    return;
                }
                disconnect(&mFsWatcher, &QFileSystemWatcher::fileChanged, this, &LoggerUtils::fileChanged);
                connect(&mFsWatcher, &QFileSystemWatcher::fileChanged, this, &LoggerUtils::fileChanged);
            }
            else {
                qWarning() << "file not exist";
            }
        }
    }
private:
    quint32 leverConverter(const QString& l){
        if (l.compare("debug") == 0)
            return 0x00000010 | 0x00000008 | 0x00000004 | 0x00000002 | 0x00000001;
        else if (l.compare("info") == 0)
            return 0x00000010| 0x00000008 | 0x00000004 | 0x00000002;
        else if (l.compare("warn") == 0)
            return 0x00000010 | 0x00000008 | 0x00000004;
        else if (l.compare("critical") == 0)
            return 0x00000010 | 0x00000008;
        else if (l.compare("fatal") == 0)
            return 0x00000010;
        else
            return 0x00000002 | 0x00000001;
    }
private:
    QString mOldLevel;
    QString mSysPath;
    QFileSystemWatcher mFsWatcher;
    TinyLogger *mLogger;
};


#endif // LOGGERUTILS_H
