#ifndef UTILITY_H
#define UTILITY_H

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

#define BOSS_PROCESS_NAME "bosssoft-assistant"

namespace DAEMON_UTILITY
{
    inline QString getAbstractPath(const QString& relativedPath)
    {

           if(QDir::isAbsolutePath(relativedPath))
           {
               return relativedPath;
           }

           QString _path;
           if(relativedPath.startsWith("./") || relativedPath.startsWith(".\\"))
           {
              _path = relativedPath.mid(2, -1);
           }

           QDir _workspace(QCoreApplication::applicationDirPath());
           return _workspace.absoluteFilePath(relativedPath);
    }

    inline bool findBossExe()
    {
        FILE* f = popen("ps -aux | grep bosssoft-assistant | grep -v grep", "r");
        if (!f)
            return false;
        char buf[256] = {0};
        fgets(buf, 255, f);
        pclose(f);
        return strlen(buf) != 0;
    }

    inline pid_t getPidByName(const QString& proc_name)
    {
        pid_t pid = -1;
        FILE *fp;
        char buf[100];
        char cmd[200] = {'\0'};
        sprintf(cmd, "pidof %s", proc_name.toStdString().c_str());

        if((fp = popen(cmd, "r")) != nullptr)
        {
            if(fgets(buf, 255, fp) != nullptr && strlen(buf) > 0)
            {
                pid = atoi(buf);
            }
        }

        pclose(fp);

        return pid;
    }

    inline int excuteShell(const QString& cmdline)
    {
        FILE *fp = nullptr;
        char buf[100] = {0};

        if((fp = popen(cmdline.toStdString().c_str(), "r")) == nullptr)
            return -1;
        if(fgets(buf, 255, fp) == nullptr ){
            pclose(fp);
            return -1;
        }
        pclose(fp);
        return 0;
    }

    inline int killBossExe(bool bForce = false)
    {
        pid_t pid = getPidByName(BOSS_PROCESS_NAME);
        if (pid == -1)
            return true;
        if (bForce){
            return kill(pid, SIGKILL);
        }
        else{
            return kill(pid, SIGTERM);
        }
    }

    inline QVariant getConfigFromIni(const QString& file, const QString& key, const QVariant& defaultValue = "")
    {
        if (!QFileInfo::exists(file))
            return  defaultValue;
        QSettings sets(file, QSettings::IniFormat);
        if (!sets.contains(key))
            return  defaultValue;
        return  sets.value(key, defaultValue);
    }
}


#endif // UTILITY_H
