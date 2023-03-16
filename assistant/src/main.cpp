#include "mainwindow.h"
#include <QDir>
#include <QApplication>
#include <QSettings>
#include <QTextCodec>
#include "httplistener.h"
#include "requestmapper.h"
#include "configmanager.h"
#include "systemtray.h"
#include "businessservice.h"
#include <QTranslator>
#include "designersettingmanager.h"
#include <qtsingleapplication.h>
#include <QFont>
#include <QFontDatabase>
#include "utility.h"
#include "loggerutils.h"

using namespace stefanfrings;

#define LOG_FILE  "bosssoft_assistant_log.log"
#define LOG_FILE_PREFIX "bosssoft_assistant_"
#define LOG_FILE_SUFFIX ".log"
static QSharedPointer<LoggerUtils> glogger;


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


int main(int argc, char *argv[])
{
    QString appName = "bosssoft-assistant.program";
    QtSingleApplication app(appName, argc, argv);
    //QApplication app(argc, argv);
    if (app.isRunning()) {
        qWarning()<<"bosssoft-assistant is already running.";
        //app.sendMessage("raise", 4000);
        return EXIT_SUCCESS;
    }
    QtSingleApplication::setQuitOnLastWindowClosed(false);

    DesignerSettingManager manager;
    QTranslator limeReportTranslator;
    QTranslator qtBaseTranslator;
    QTranslator qtDesignerTranslator;
    QTranslator qtLinguistTranslator;
    QTranslator translator;

    QString translationPath = QApplication::applicationDirPath();
    translationPath.append("/translations");
    Qt::LayoutDirection layoutDirection = QLocale::system().textDirection();
    app.setLayoutDirection(layoutDirection);

    QString designerTranslation = QLocale(manager.getCurrentDefaultLanguage()).name();

    if (limeReportTranslator.load("limereport_"+designerTranslation, translationPath)){
        app.installTranslator(&limeReportTranslator);

        Qt::LayoutDirection layoutDirection = QLocale(manager.getCurrentDefaultLanguage()).textDirection();
        app.setLayoutDirection(layoutDirection);
    }

    translationPath.append("/sys");
    QTranslator translator1;
    translator1.load("qt_zh_CN", translationPath);
    app.installTranslator(&translator1);
    QTranslator translator2;
    translator2.load("assistant_zh_CN", translationPath);
    app.installTranslator(&translator2);
    QTranslator translator3;
    translator3.load("designer_zh_CN", translationPath);
    app.installTranslator(&translator3);
    QTranslator translator4;
    translator4.load("linguist_zh_CN", translationPath);
    app.installTranslator(&translator4);
    QTranslator translator5;
    translator5.load("qtbase_zh_CN", translationPath);
    app.installTranslator(&translator5);
    QTranslator translator6;
    translator6.load("qt_help_zh_CN", translationPath);
    app.installTranslator(&translator6);

#if 1
    QString dirPath = QApplication::applicationDirPath();
    dirPath.append("/ttf");
    QDir dir(dirPath);
    QStringList nameFilters;
    nameFilters<<"*.ttf";
    nameFilters<<"*.ttc";
    QStringList files = dir.entryList(nameFilters, QDir::Files| QDir::Readable, QDir::Name);
    QString str;
    QString fontPath;
    int fontId;
    foreach(str, files) {
        if (str.isEmpty()) {
            continue;
        }
        fontPath = dirPath + "/" + str;
        fontId = QFontDatabase::addApplicationFont(fontPath);
        //qDebug()<<fontId;
#if 0
        if (fontId != -1) {
            QStringList fontList(QFontDatabase::applicationFontFamilies(fontId));
            //qDebug()<<fontList;
            if (fontList.count() > 0) {
                QFont font_zh(fontList.at(0));
                font_zh.setBold(false);
                app.setFont(font_zh);
            }
        }
#endif
    }
#endif

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#if _MSC_VER
    QTextCodec *codec = QTextCodec::codecForName("gbk");
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
#endif
    if (glogger.isNull())
        glogger.reset(new LoggerUtils());
    glogger->initLog();
    qInstallMessageHandler(LogHandler);

    //qWarning()<<qEnvironmentVariable("LD_LIBRARY_PATH","default");

    if(!ConfigManager::getInstance()->load())
    {
        qFatal("load config path failure! reason =%s", qPrintable(ConfigManager::getInstance()->errorReason()));
        return -1;
    }

    BusinessService bsService;
    bsService.initService();

    /// for http connection.
    QSettings* listenerSettings=new QSettings(ConfigManager::getInstance()->httpConnectionConfigFilePath(), QSettings::IniFormat,&app);
    listenerSettings->beginGroup("listener");

    QSharedPointer<HttpListener> _listener(new HttpListener(listenerSettings,new RequestMapper(&app),&app));

    /// for https connection.
    QSettings* listenerSettingsHttps=new QSettings(ConfigManager::getInstance()->httpConnectionConfigFilePath(), QSettings::IniFormat,&app);
    listenerSettingsHttps->beginGroup("listenerhttps");

    QSharedPointer<HttpListener> _listenerhttps(new HttpListener(listenerSettingsHttps,new RequestMapper(&app),&app));

    SystemTray _tray;
    _tray.hide();

    return app.exec();
}
