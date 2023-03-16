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
        :mSysPath(DAEMON_UTILITY::getAbstractPath("conf/tinylogger.ini")),
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
        logDir = DAEMON_UTILITY::getConfigFromIni(mSysPath, "TinyLogger/workspace" , DAEMON_UTILITY::getAbstractPath("log/daemon")).toString();
        logName = DAEMON_UTILITY::getConfigFromIni(mSysPath, "TinyLogger/baseFileName" , "daemon").toString();
        logExtern = DAEMON_UTILITY::getConfigFromIni(mSysPath, "TinyLogger/fileNameExtension" , ".log").toString();
        mOldLevel = DAEMON_UTILITY::getConfigFromIni(mSysPath, "TinyLogger/level" , "info").toString().toLower();
        size_t maxSize = DAEMON_UTILITY::getConfigFromIni(mSysPath, "TinyLogger/maxFileSize" , "10").toUInt() * 1024 * 1024;
        size_t maxNum = DAEMON_UTILITY::getConfigFromIni(mSysPath, "TinyLogger/maxFileNum" , "3").toUInt();

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
            QString level = DAEMON_UTILITY::getConfigFromIni(mSysPath, "TinyLogger/level" , "").toString().toLower();
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
