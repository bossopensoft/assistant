/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include <QMap>
#include <QMutex>
#include <QByteArray>
#include <QDebug>
#include <QMutexLocker>

#include "requestmapper.h"

#include "icontroller.h"
#include "appctrlcontroller.h"
#include "nullcontroller.h"
#include "businessservice.h"
#include "upgradecontroller.h"

static QMutex mapperMutex;

static inline ControllerDecorator* make_controller(IController* controller)
{
    ControllerDecorator* _decorator =new ControllerDecorator(controller);
    return _decorator;
}

const QMap<QString, ControllerDecorator*> ControllerMap=
{
    {"/"+ MODULE_APPCTRL,     make_controller(new AppctrlController())},
    {"/"+ MODULE_UPGRADE,     make_controller(new UpgradeController())}
};

static bool callingService(const QString path, HttpRequest& request, HttpResponse& response) // thread safe method.
{
    ControllerDecorator *_controller =nullptr;

    QMutexLocker __autoLocker(&mapperMutex);
    foreach (QString _controllerKey, ControllerMap.keys())
    {
        if(!path.startsWith(qPrintable(_controllerKey))) continue;

        _controller =ControllerMap[_controllerKey];
        break;
    }
    __autoLocker.unlock();

    if (_controller == nullptr) {
        return true;
    }
#if 1
    if(_controller->isBusy())
    {
        qWarning()<<"call controller "<<_controller->name()<<" but it was busy!";
        BusinessService myBusinessService;
        response.write(myBusinessService.setJsonString("2010", QObject::tr("前一个打印流程未结束，请先关闭打印窗口")));
        return false;
    }
#endif
    if(!_controller->service(request, response))
    {
        qWarning()<<"called controller "<< _controller->name()<<" was failure!";
    }

    return true;
}


RequestMapper::RequestMapper(QObject* parent)
    :HttpRequestHandler(parent)
{
    qDebug("RequestMapper: created");
}


RequestMapper::~RequestMapper()
{
    qDebug("RequestMapper: deleted");
}


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path =request.getPath();

    qDebug("RequestMapper: path =%s", path.data());

    callingService(path, request, response);//critial section.

    qDebug("RequestMapper: finished request");
}
