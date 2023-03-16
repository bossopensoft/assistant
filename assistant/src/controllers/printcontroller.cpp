#include "printcontroller.h"
#include "businessservice.h"
#include <QDir>
#include "configmanager.h"
#include "inifilehandler.h"
#include <QCoreApplication>

PrintController::PrintController()
{

}

bool PrintController::service(HttpRequest &request, HttpResponse &response)
{
    BusinessService bsvr;
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    QString id = request.getParameter(REQUEST_PARAM_KEY_ID.toLocal8Bit());
    QString func = request.getParameter(REQUEST_PARAM_KEY_FUNC.toLocal8Bit());
    if (func.isEmpty()){
        response.write(bsvr.setJsonString("400", QObject::tr("PRINT方法不能为空")));
        return false;
    }
#if 0
    QString _filePath =ConfigManager::getInstance()->getSysConfigManager()->filePath();
    QSharedPointer<IniFileHandler> tmpFileHandler(new IniFileHandler(_filePath));
    if (tmpFileHandler->exist()) {
        tmpFileHandler->setValue("/HTTP/server.openssl", "0");
    }
#endif
    QByteArray payload = request.getParameter(REQUEST_PARAM_KEY_PAYLOAD.toLocal8Bit());
    if (payload.isEmpty()){
        response.write(bsvr.setJsonString("400", QObject::tr("PRINT方法不能为空")));
        return false;
    }

    payload = bsvr.doBase64Decode(payload, 0);
    QByteArray resJson = bsvr.serviceEntry(QCoreApplication::applicationDirPath(), payload, QString(MODULE_PRINT), func);

    if (!bsvr.isAgency3Intfc(payload)){
        QByteArray bScript = bsvr.ReadScriptText();
        response.write(bsvr.BuildResponseScript(id, bScript, resJson));
    } else {
        response.write(resJson);
    }

    return true;
}
