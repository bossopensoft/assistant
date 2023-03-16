/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include <QMap>
#include <QMutex>
#include <QByteArray>
#include <QDebug>
#include <QMutexLocker>

#include "requestmapper.h"
#include "filelogger.h"

#include "icontroller.h"
#include "heartcontroller.h"
#include "nullcontroller.h"
#include "printcontroller.h"
#include "cacontroller.h"
#include "updatecontroller.h"
#include "uploadcontroller.h"
#include "businessservice.h"
#include "previewimgcontroller.h"
#include "appctrlcontroller.h"
#include "qrcodepaycontroller.h"
#include "poscontroller.h"

/*
const QString MODULE_HEART          = "heart";      // 心跳
const QString MODULE_PREVIEW_IMAGE  = "preview";    // own
const QString MODULE_SIGN           = "sign";       // 兼容旧客户端
const QString MODULE_UPDATE         = "update";     // 客户端更新地址
const QString MODULE_PRINT          = "print";      // 打印

const QString MODULE_UPLOAD         = "upload";     // 上传
const QString MODULE_CA             = "ca";         // 签名
const QString MODULE_POS            = "pos";        // pos刷卡
const QString MODULE_PD             = "pd";         //
const QString MODULE_BSREMOTE       = "NontaxAgencyActuator";
const QString MODULE_CAMERA         = "camera";     // 高拍仪
const QString MODULE_APPCTRL        = "appctrl";    // 与客户端服务交互
const QString MODULE_SMART_CARD     = "smart-card"; // 智能卡
const QString MODULE_WEBBILLCARD    = "webbillcard";// WEB票据智能卡
const QString MODULE_EXTEND         = "extend"; // 通用扩展接口
const QString MODULE_QRCODEPAY      = "qrcodepay"; // 扫码支付终端
const QString MODULE_BROWSEREXTEND  = "browser-extend"; //浏览器通用拓展接口
*/

static QMutex mapperMutex;

static inline ControllerDecorator* make_controller(IController* controller)
{
    ControllerDecorator* _decorator =new ControllerDecorator(controller);
    return _decorator;
}

const QMap<QString, ControllerDecorator*> ControllerMap=
{
    {"/"+ MODULE_HEART,             make_controller(new HeartController())},
    {"/"+ MODULE_PRINT,             make_controller(new PrintController())},
    {"/"+ MODULE_CA,                make_controller(new CaController())},
    {"/"+ MODULE_UPDATE,            make_controller(new UpdateController())},
    {"/"+ MODULE_UPLOAD,            make_controller(new UploadController())},
    {"/"+ MODULE_PREVIEW_IMAGE,     make_controller(new PreviewImgController())},
    {"/"+ MODULE_APPCTRL,           make_controller(new AppctrlController())},
    {"/"+ MODULE_QRCODEPAY,         make_controller(new QRcodePayController())},
    {"/"+ MODULE_POS,               make_controller(new PosController())},
};

static bool callingService(const QString path, HttpRequest& request, HttpResponse& response) // thread safe method.
{
    ControllerDecorator *_controller =nullptr;

    QMutexLocker __autoLocker(&mapperMutex);
    foreach (QString _controllerKey, ControllerMap.keys())
    {
        if(!path.startsWith(qPrintable(_controllerKey))) continue;

        _controller =ControllerMap[_controllerKey];
        break;
    }
    __autoLocker.unlock();

    if (_controller == nullptr) {
        return true;
    }
#if 1
    if(_controller->isBusy())
    {
        qWarning()<<"call controller "<<_controller->name()<<" but it was busy!";
        BusinessService callBusinessService;
        if (_controller->name() == "heart controller") {
            response.setHeader("Content-Type", "text/html; charset=UTF-8");
            response.write(callBusinessService.setJsonString("400", QObject::tr("前一个操作流程未结束，请先关闭窗口")));
        } else {
            response.setHeader("Content-Type", "text/html; charset=UTF-8");
            QString id = request.getParameter(REQUEST_PARAM_KEY_ID.toLocal8Bit());
            QByteArray resJson = callBusinessService.setJsonString("400", QObject::tr("前一个操作流程未结束，请先关闭窗口"));
            QByteArray bScript = callBusinessService.ReadScriptText();
            response.write(callBusinessService.BuildResponseScript(id, bScript, resJson));
        }

        return false;
    }
#endif
    if(!_controller->service(request, response))
    {
        qWarning()<<"called controller "<< _controller->name()<<" was failure!";
    }

    return true;
}
static void setHttpSymbol(QByteArray host)
{
    QString port, convString, value;
    convString = QString(host);
    port = convString.section(':', 1, 1);

    BusinessService busiServ;
    if (port == busiServ.readIniSys("HTTP", "server.port", "13526")) {
        value = "0";
    } else {
        value = "1";
    }

    busiServ.writeIniSys("HTTP", "server.openssl", value);
}

RequestMapper::RequestMapper(QObject* parent)
    :HttpRequestHandler(parent)
{
    qInfo("RequestMapper: created");
}


RequestMapper::~RequestMapper()
{
    qInfo("RequestMapper: deleted");
}


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path =request.getPath();

    qInfo("RequestMapper: path =%s", path.data());

    QByteArray host =request.getHeader("host");
    setHttpSymbol(host);

    callingService(path, request, response);//critial section.

    qInfo("RequestMapper: finished request");
}
