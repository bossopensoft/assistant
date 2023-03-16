#include <QCoreApplication>
#include <QDebug>

#include "programlauncher.h"
#include "../utility.h"
#include <QApplication>
#include <QSettings>

ProgramLaucher* ProgramLaucher::mInstance =nullptr;

ProgramLaucher::ProgramLaucher(const QString commandline, QObject *parent)
               : QObject(parent)
               , mExecutor(nullptr)
               , mCommandline(commandline)
               , mExitCode(0)
{

}

ProgramLaucher::~ProgramLaucher()
{
    if(mExecutor) delete  mExecutor;
    mExecutor = nullptr;
}

bool ProgramLaucher::start()
{
    mExitCode = 0;
    mEnv = QProcessEnvironment::systemEnvironment();
    QString path = QString("%1:%2:%3")
            .arg(DAEMON_UTILITY::getAbstractPath("../dep/lib"))
            .arg(DAEMON_UTILITY::getAbstractPath("DLL/3rdparty/boss-report"))
            .arg(QString::fromLatin1(qgetenv("LD_LIBRARY_PATH")));
    mEnv.insert("LD_LIBRARY_PATH", path);

    if(mExecutor) delete  mExecutor;
    //下一句括号留空，防止提示跨线程调用
    mExecutor = new QProcess();
    mExecutor->setProcessEnvironment(mEnv);
    bind(mExecutor);
    //qWarning()<<"mExecutor->processEnvironment(): "<<mExecutor->processEnvironment().toStringList();

    mExecutor->start(mCommandline);
    return mExecutor->waitForStarted();
}

QProcess::ProcessError ProgramLaucher::error() const
{
    return mExecutor->error();
}

QString ProgramLaucher::program() const
{
    return mExecutor->program();
}

void ProgramLaucher::bind(QProcess *executor)
{
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
    QObject::connect(executor, SIGNAL(started()), this, SLOT(onStart()));
    QObject::connect(executor, SIGNAL(finished(int , QProcess::ExitStatus )),
                     this,   SLOT(onFinished(int , QProcess::ExitStatus )));
}

void ProgramLaucher::onStart()
{
    qWarning()<<"application started! ";
}

void ProgramLaucher::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qWarning()<<"quit application exit code = "<<exitCode;
    qWarning()<<"quit application exit status = "<<exitStatus;
    QString sysSettingsPath = QApplication::applicationDirPath() + "/conf/sys.ini";
    QSettings settingsRead(sysSettingsPath, QSettings::IniFormat);
    bool crashed = true;
    if (settingsRead.value("APP/application.exit").toString() == "11111") {
        crashed = false;
        QSettings settings(sysSettingsPath,QSettings::IniFormat);
        settings.beginGroup("APP");
        settings.setValue("application.exit", "0");
    }

    if (exitCode != 0 && crashed == true) {
        start();
    }
#if 0
    if(exitCode == 252)
    {
        // restart the previous commandline.
        start();
    }
#endif
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);

    //qInfo()<<"quit application exit code= "<<exitCode;
//    qApp->quit();
}
