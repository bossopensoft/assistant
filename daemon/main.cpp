#include <QCoreApplication>
#include <QtGlobal>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QProcess>
#include <QDateTime>
#include <QThread>
#include <QSharedPointer>

#include <strings.h>
#include <errno.h>
#include <array>
#include <functional>
#include <stdio.h>
#include <strstream>

#include "httplistener.h"
#include "requestmapper.h"
#include "programlauncher.h"
#include "utility.h"
#include "qtsingleapplication.h"
#include "loggerutils.h"

#define LOG_FILE  "bosssoft_daemon_log.log"
#define LOG_FILE_PREFIX "bosssoft_daemon_"
#define LOG_FILE_SUFFIX ".log"

//static QString glogFilePath="";
static QSharedPointer<LoggerUtils> glogger;
/*
typedef void (*QtMessageHandler)(QtMsgType, const QMessageLogContext &, const QString &);
Q_CORE_EXPORT QtMessageHandler qInstallMessageHandler(QtMessageHandler);
*/

void LogHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    auto __get_current_dateTime=[]()->QString
    {
        // qint64 passedTimeInMS= QDateTime::currentMSecsSinceEpoch() - QDateTime(QDate::currentDate()).toMSecsSinceEpoch();
        return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    };

    auto __get_readableMessageType=[](QtMsgType typeNo)->QString
    {
        const char* _type_texts[]={"Debug","Warning","Critical","Fatal","Info"};
        return QString::fromLatin1(_type_texts[(int)(typeNo)]);
    };


    auto __get_template=[]()->QString
    {
        QString _template={"%1:[%2 @F=%3,fn=%4,ln=%5] %6%7"};
        return _template;
    };

    QByteArray lMessage = message.toLocal8Bit();

    QString text=__get_template().arg(__get_readableMessageType(type))
                                 .arg(__get_current_dateTime())
                                 .arg(context.file)
                                 .arg(context.function)
                                 .arg(context.line)
                                 .arg(lMessage.constData())
                                 .arg("\n");

    if (!glogger.isNull() && glogger->getLogger())
        glogger->getLogger()->writeLogger(glogger->MsgTypeConverter(type), text.toStdString().c_str());
}

static inline QString __toReadableString(QProcess::ProcessError _errno)
{
    const char* __error_strings[]=
    {
        "Failed To Start",
        "Crashed",
        "Timedout",
        "Read Error",
        "Write Error",
        "Unknown Error"
    };

    const QString __internalReason= QString::fromLatin1(strerror(_errno));

    if(_errno < QProcess::FailedToStart || _errno > QProcess::UnknownError)
    {
        _errno = QProcess::UnknownError;
    }

    return QString::fromLatin1(__error_strings[_errno]) +"internal reason: " + __internalReason;
}

static void __dump_argc(int& argc, char* argv[])
{
    printf("total args=%d \n", argc);

    for(int i=0; i!= argc; i++)
    {
        printf("arg[%d] =%s \n",i, argv[i]);
    }
}

static int __startMainProgram(QString* errMsg)
{
    const QString __program_name="bosssoft-assistant";
    const QString __path = QCoreApplication::applicationDirPath()+"/"+__program_name;
    //qDebug()<<"client path: "<<__path;

    //qWarning()<<"LD_LIB : "<<qgetenv("LD_LIBRARY_PATH");

    ProgramLaucher::getInstance()->setProgramName(__path);

    //    ProgramLaucher _launcher(__path);

    if (!ProgramLaucher::getInstance()->start()) {
        *errMsg = __toReadableString(ProgramLaucher::getInstance()->error());
        //qWarning()<<"launcher err!";
        return -1;
    }
    *errMsg = "";
    return 0;
}

int main(int argc, char *argv[])
{
    QString appName = "bosssoft-assistant.daemon";
    QtSingleApplication a(appName, argc, argv);
    QString errMsg;
    int ret;

    //QApplication app(argc, argv);
    //ret = __startMainProgram(&errMsg);
    if (a.isRunning()) {
        qWarning()<<"daemon is already running.";
        ret = a.sendMessage("active app", 4000);
        qWarning()<<"new daemon send active to old daemon, result :"<<ret;
        return EXIT_SUCCESS;
    } else {
        a.bindSlots();
    }
    //QCoreApplication a(argc, argv);

//    __dump_argc(argc, argv);

    if (glogger.isNull())
        glogger.reset(new LoggerUtils());
    glogger->initLog();
    qInstallMessageHandler(LogHandler);

    ret = __startMainProgram(&errMsg);
    //QString currentFilePath = QString::fromLatin1(argv[0]);
    if (ret != 0) {
        qWarning()<<"startMainProgram : "<<errMsg;
        return ret;
    }
    //http server
    QString tt = DAEMON_UTILITY::getAbstractPath("conf/sys.ini");
    QSettings* listenerSettings=new QSettings(DAEMON_UTILITY::getAbstractPath("conf/sys.ini"), QSettings::IniFormat,&a);
    listenerSettings->beginGroup("svrlistener");

    QSharedPointer<HttpListener> _listener(new HttpListener(listenerSettings,new RequestMapper(&a),&a));

    /// for https connection.
    QSettings* listenerSettingsHttps=new QSettings(DAEMON_UTILITY::getAbstractPath("conf/sys.ini"), QSettings::IniFormat,&a);
    listenerSettingsHttps->beginGroup("svrlistenerhttps");

    QSharedPointer<HttpListener> _listenerhttps(new HttpListener(listenerSettingsHttps,new RequestMapper(&a),&a));

    qWarning()<<"executing command line: "<<ProgramLaucher::getInstance()->program()<<" is oks";
    return a.exec();
}
