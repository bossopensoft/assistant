//#include "mainwindow.h"

#include "systemtray.h"
#include <QApplication>
#include <QtWidgets/QApplication>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    SystemTray w;
    w.hide();
    int e;
    e = a.exec();
    if (e == RETCODE_APP_RESTART) {
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        return 0;
    }
    return e;
}
