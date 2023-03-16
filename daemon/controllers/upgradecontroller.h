#ifndef UPGRADECONTROLLER_H
#define UPGRADECONTROLLER_H

#include "icontroller.h"

class UpgradeController : public IController {
public:
    UpgradeController();
    virtual QString name()const{return "upgrade app controller";}
    virtual bool    service(HttpRequest& request, HttpResponse& response);
};

#endif // UPGRADECONTROLLER_H
