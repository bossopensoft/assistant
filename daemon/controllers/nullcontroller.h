#ifndef NULLCONTROLLER_H
#define NULLCONTROLLER_H


#include "icontroller.h"
#include <QDebug>

class NullController : public IController
{
public:
    NullController();
    virtual QString name()const{return "null-controller";}
    virtual bool    service(HttpRequest& request, HttpResponse& response){qWarning()<<"called "<<name()<<".service() in failure!"; return false;}
};

#endif // NULLCONTROLLER_H
