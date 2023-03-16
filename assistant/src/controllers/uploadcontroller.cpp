#include "uploadcontroller.h"
#include <QCoreApplication>
#include "businessservice.h"
UploadController::UploadController()
{

}

bool UploadController::service(HttpRequest &request, HttpResponse &response)
{
    BusinessService bsvr;
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    QString id = request.getParameter(REQUEST_PARAM_KEY_ID.toLocal8Bit());
    QString func = request.getParameter(REQUEST_PARAM_KEY_FUNC.toLocal8Bit());
    if (func.isEmpty()){
        response.write(bsvr.setJsonString("400", QObject::tr("UPLOAD方法不能为空")));
        return true;
    }

    QString isDoubleBase = request.getParameter(REQUEST_PARAM_KEY_DOUBLEBASE.toLocal8Bit());
    QByteArray payload = request.getParameter(REQUEST_PARAM_KEY_PAYLOAD.toLocal8Bit());
    if (payload.isEmpty()){
        response.write(bsvr.setJsonString("400", QObject::tr("UPLOAD参数不能为空")));
        return true;
    }

    payload = bsvr.doBase64Decode(payload, 0);
    if (!isDoubleBase.isEmpty())
        payload = bsvr.doBase64Decode(payload, 0);

    QByteArray resJson = bsvr.serviceEntry(QCoreApplication::applicationDirPath(), payload, QString(MODULE_UPLOAD), func);

    if (!bsvr.isAgency3Intfc(payload)){
        QByteArray bScript = bsvr.ReadScriptText();
        response.write(bsvr.BuildResponseScript(id, bScript, resJson));
    }
    else{
        response.write(resJson);
    }
    return true;
}
