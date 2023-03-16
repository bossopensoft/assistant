#include "appctrlcontroller.h"
#include <QApplication>
AppctrlController::AppctrlController()
{

}
bool AppctrlController::service(HttpRequest &request, HttpResponse &response)
{
    // TODO implement later.
    QString value = request.getParameter("cmd");
    if (value.isEmpty())
        response.setStatus(400, "not found cmd pamram");
    if (value.compare("1") == 0){
        qApp->exit(0);
    }
    else {
        response.setStatus(400, "unknown cmd pamram");
    }
    return true;
}
