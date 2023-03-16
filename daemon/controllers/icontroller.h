#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <QString>

#include <QMutex>
#include <QMutexLocker>
#include "httprequesthandler.h"

using namespace stefanfrings;

class IController
{
public:
    virtual ~IController(){}

    virtual QString name()const =0;
    virtual bool    service(HttpRequest& request, HttpResponse& response)=0;
};


class ControllerDecorator : public IController
{
public:
    ControllerDecorator(IController* controller):mBusy(false),mParentController(controller){}

    virtual ~ControllerDecorator()
    {
        if(mParentController) delete  mParentController;
    }

    virtual bool service(HttpRequest &request, HttpResponse &response) override
    {
       StatusGuard _guard(&mBusy);
       QMutexLocker _autoLocker(&mMutex);

       return mParentController->service(request, response);
    }

    virtual QString name()const override{return mParentController->name();}

    bool isBusy()const{return mBusy;}

private:
    class StatusGuard
    {
    public:
        StatusGuard(volatile bool* status):mStatus(status){*mStatus=true;}
        ~StatusGuard(){*mStatus = false;}
    private:
        volatile bool* mStatus;
    };

    volatile bool         mBusy;
    IController*          mParentController;
    QMutex                mMutex;
};

#endif // ISERVICE_H
