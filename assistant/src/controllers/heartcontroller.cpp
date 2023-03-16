#include "heartcontroller.h"

#define HEART_REQ_KEY               ("jsonp")
#define HEART_RSP_DEFUALT_VALUE     ("(0)")

HeartController::HeartController()
{

}

bool HeartController::service(HttpRequest &request, HttpResponse &response)
{
    // TODO implement later.
    QByteArray value;
    QString rsp_data;
    value = request.getParameter(HEART_REQ_KEY);
    rsp_data = value;
    if(rsp_data.isEmpty()) {
        rsp_data = HEART_RSP_DEFUALT_VALUE;
    } else {
        rsp_data = rsp_data + HEART_RSP_DEFUALT_VALUE;
    }
    response.write(rsp_data.toLatin1());

    return true;
}
