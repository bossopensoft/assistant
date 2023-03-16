#include "poscontroller.h"
#include "businessservice.h"
#include <QCoreApplication>

PosController::PosController()
{

}

bool PosController::service(HttpRequest &request, HttpResponse &response)
{
    BusinessService bsvr;
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    QString id = request.getParameter(REQUEST_PARAM_KEY_ID.toLocal8Bit());
    QString func = request.getParameter(REQUEST_PARAM_KEY_FUNC.toLocal8Bit());
    if (func.isEmpty()){
        response.write(bsvr.setJsonString("400", QObject::tr("POS方法不能为空")));
        return false;
    }

    QByteArray payload = request.getParameter(REQUEST_PARAM_KEY_PAYLOAD.toLocal8Bit());
    if (payload.isEmpty()){
        response.write(bsvr.setJsonString("400", QObject::tr("POS方法不能为空")));
        return false;
    }

    payload = bsvr.doBase64Decode(payload, 0);
    payload.replace("\\\"", "\"");
    payload.replace("\"{", "{");
    payload.replace("}\"", "}");
    QByteArray resJson = bsvr.serviceEntry(QCoreApplication::applicationDirPath(), payload, QString(MODULE_POS), func);

    if (!bsvr.isAgency3Intfc(payload)){
        QByteArray bScript = bsvr.ReadScriptText();
        response.write(bsvr.BuildResponseScript(id, bScript, resJson));
    } else {
        response.write(resJson);
    }

    return true;
}
