#ifndef PROGRAME_LAUNCHER_H
#define PROGRAME_LAUNCHER_H

#include <QObject>
#include <QProcess>
#include <QMetaType>

class ProgramLaucher : public QObject
{
    Q_OBJECT
public:
    explicit ProgramLaucher(const QString program="", QObject *parent=nullptr);
    virtual ~ProgramLaucher();
    bool start();
    QProcess::ProcessError error()const;
    QString program()const;
    void setProgramName(const QString& program)
    {
        mCommandline = program;
    }
    int exitCode()const{return mExitCode;}
#if 1
    static ProgramLaucher* getInstance()
    {
        if(mInstance) return mInstance;
        mInstance = new  ProgramLaucher();
        return mInstance;
    }
#endif
#if 0
    static ProgramLaucher & getInstance()
    {
        static ProgramLaucher instance;
        return instance;
    }
#endif
signals:
    void sigQuit();

private slots:
    void onStart();
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    void bind(QProcess *client);

    QProcess* mExecutor;
    QString   mCommandline;
    QProcessEnvironment mEnv;
    int mExitCode;
    static  ProgramLaucher* mInstance;
};

#endif // CLIENTOBSERVER_H
