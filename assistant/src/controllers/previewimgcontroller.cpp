#include "previewimgcontroller.h"
#include <QCoreApplication>
#include <QFileInfo>

#define PREVIEW_KEY_FN                      ("fn")
#define PREVIEW_IMAGE_SAVE_RELATIVE_DIR     ("/data/img/preview/")

PreviewImgController::PreviewImgController()
{

}

bool PreviewImgController::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray value;
    QString fn, fp;
    value = request.getParameter(PREVIEW_KEY_FN);
    fn = value;
    fp = QCoreApplication::applicationDirPath() + PREVIEW_IMAGE_SAVE_RELATIVE_DIR +fn;
    QFileInfo file(fp);
    if (file.exists() == true) {
        response.setHeader("Content-Type", "image/png; charset=UTF-8");
        QString content = "attachment; filename=" + fn;
        response.setHeader("Content-Disposition", content.toLocal8Bit());
        QFile loadFile(fp);
        loadFile.open(QIODevice::ReadOnly);
        QByteArray allData = loadFile.readAll();
        loadFile.close();
        response.write(allData);
    }
    return true;
}
