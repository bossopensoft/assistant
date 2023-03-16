#include "updatecontroller.h"
#include "inifilehandler.h"
#include "configmanager.h"

#include <QSharedPointer>

#define UPDATE_REQ_KEY               ("jsonp")
#define UPDATE_RSP_DEFUALT_VALUE     ("(0)")
#define UPDATE_RSP_ERROR_VALUE       ("(-1)")
#define UPDATE_URL_KEY               ("url")
#define UPDATE_SET_URL_KEY           ("/HTTP/server.update.url")

UpdateController::UpdateController()
{

}

static bool setNewUpdateUrl(QString newUrl)
{
    if(newUrl.isEmpty()){
        return false;
    }

    QString _filePath =ConfigManager::getInstance()->getSysConfigManager()->filePath();
    QSharedPointer<IniFileHandler> tmpFileHandler(new IniFileHandler(_filePath));
    if (tmpFileHandler->exist()) {
        tmpFileHandler->setValue(UPDATE_SET_URL_KEY, newUrl+"upgrade/app");
    } else {
        return false;
    }

    return true;
}

static bool updateApp(void)
{
    // TODO implement later.
    return true;
}

bool UpdateController::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray value;
    QString rsp_data, newUrl;

    value = request.getParameter(UPDATE_REQ_KEY);
    rsp_data = value;

    value = request.getParameter(UPDATE_URL_KEY);
    newUrl = value;

    if (setNewUpdateUrl(newUrl) == true) {
        rsp_data += UPDATE_RSP_DEFUALT_VALUE;
    } else {
        rsp_data += UPDATE_RSP_ERROR_VALUE;
    }

    response.write(rsp_data.toLatin1());

    updateApp();

    return true;
}
