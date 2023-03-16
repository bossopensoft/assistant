#include "appctrlcontroller.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QProcess>
#include <QProcessEnvironment>
#include <QEventLoop>
#include "../programlauncher.h"
#include "../utility.h"
#define REQ_KEY               ("ct")

AppctrlController::AppctrlController()
{

}

bool AppctrlController::service(HttpRequest &request, HttpResponse &response)
{
    // TODO implement later.
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    QString resp = request.getParameter("jsonp");
    QString value = request.getParameter(REQ_KEY);
    if(!value.isEmpty()) {
        if (value.compare("1") == 0){
            DAEMON_UTILITY::killBossExe();
        }
        else if (value.compare("2") == 0){
            if (DAEMON_UTILITY::findBossExe()){
                QNetworkRequest req(QUrl("http://127.0.0.1:13526/appctrl?cmd=1"));
                QNetworkAccessManager m;
                QEventLoop loop;
                auto reply = m.get(req);
                QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
                loop.exec();
                reply->deleteLater();
            }
#if 0
            QProcess process;
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            env.insert("PATH", DAEMON_UTILITY::getAbstractPath("../dep/lib"));
            env.insert("PATH", DAEMON_UTILITY::getAbstractPath("DLL/3rdparty/boss-report"));
            process.setProcessEnvironment(env);
            const QString __program_name="bosssoft-assistant";
            const QString __path = QCoreApplication::applicationDirPath()+"/"+__program_name;
            process.startDetached(__path);
#endif
            resp.append("({ \"ret_code\": \"0\", \"ret_msg\" : \"博思客户端助手操作正常\" })");
            response.write(resp.toLocal8Bit());

            const QString __program_name="bosssoft-assistant";
            const QString __path = QCoreApplication::applicationDirPath()+"/"+__program_name;
            ProgramLaucher::getInstance()->setProgramName(__path);
            if(!ProgramLaucher::getInstance()->start()) {
                qWarning()<<"restart launcher err!";
                return -1;
            }

        }
        else {
            resp.append("({ \"ret_code\": \"1\", \"ret_msg\" : \"博思客户端助手操作异常\" })");
            response.write(resp.toLocal8Bit());
        }
    } else {
        resp.append("({ \"ret_code\": \"1\", \"ret_msg\" : \"博思客户端助手操作异常\" })");
        response.write(resp.toLocal8Bit());
    }
    return true;
}
