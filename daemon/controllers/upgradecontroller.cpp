#include "upgradecontroller.h"
#include <QStringList>
#include <QProcess>
#include <QFile>
#include <QtDebug>
#include <QCoreApplication>
#include "../utility.h"

#define SH_NAME "run_upgrade_package.sh"
UpgradeController::UpgradeController()
{

}

bool UpgradeController::service(HttpRequest &request, HttpResponse &response)
{
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    QString resp = request.getParameter("jsonp");
    QString value = request.getParameter("fp");
    value.replace("%20", " ");
    if(!value.isEmpty()) {
        if (!QFile::exists(value)){
            qCritical() << "upgrade package not exists: " << value;
            resp.append("({ \"ret_code\": \"1\", \"ret_msg\" : \"博思客户端助手操作异常\" })");
            response.write(resp.toLocal8Bit());
            return true;
        }
        QString str = QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg(SH_NAME);
        QProcess::startDetached("/bin/bash", QStringList() << str << value);
        resp.append("({ \"ret_code\": \"0\", \"ret_msg\" : \"博思客户端助手操作正常\" })");
        response.write(resp.toLocal8Bit());
//        pid_t p = fork();
//        if (p == 0){
////            DAEMON_UTILITY::excuteShell("/home/uos/Desktop/workspace/nontax-client-qt/build/bosssoft-assistant");

////            execl("/home/uos/Desktop/workspace/nontax-client-qt/build/bosssoft-assistant", "&", nullptr);
//            exit(0);
//        }
//        QProcess::startDetached(str, {value});
        //        DAEMON_UTILITY::excuteShell(str);
    }
    else {
        resp.append("({ \"ret_code\": \"1\", \"ret_msg\" : \"博思客户端助手操作异常\" })");
        response.write(resp.toLocal8Bit());
    }
    return true;
}
