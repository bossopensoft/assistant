#ifndef HEARTCONTROLLER_H
#define HEARTCONTROLLER_H

#include "icontroller.h"
#include "../programlauncher.h"

class AppctrlController : public IController {
public:
    AppctrlController();
    virtual QString name()const{return "appctrl controller";}
    virtual bool    service(HttpRequest& request, HttpResponse& response);
};

#endif // HEARTCONTROLLER_H
