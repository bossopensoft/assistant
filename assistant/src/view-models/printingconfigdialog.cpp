#include "printingconfigdialog.h"
#include "ui_printingconfigdialog.h"
#include <QtPrintSupport/QPrinterInfo>
#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include "businessservice.h"

const QString PRINTER_SET_RELATIVE_PATH             = "/data/json/print.json";
const QString PRINTER_BILL_CODE_RELATIVE_PATH       = "/data/json/bill.json";
const QString APPLICATION_SAVE_RELATIVE_PATH        = "/data/json/application.json";
const QString DLG_NODE_MODULE_PRINTER               = "_PRINTER";
const QString DLG_NODE_PRINTER                      = "00000000_PRINTER";
const QString DLG_DEFAULT_MODULE_ID                 = "00000000";
const QString DLG_DEFAULT_MODULE_MARGIN             = "0";
//const QString DEFAULT_PRINTER_PAPER = "(自定义纸张)";
const QString DLG_DEFAULT_PAPER_ORIENTATION = "纵向";
const QString DLG_DEFAULT_PAPER_SIZE    = "A4";
const QString DLG_DEFAULT_PAPER_OFFSET  = "0";
const QString DLG_DEFAULT_PRINTER_NAME  = "PDF";

PrintingConfigDialog::PrintingConfigDialog(QWidget *parent)
    //: QDialog(parent, Qt::FramelessWindowHint | Qt::Dialog)
    : QDialog(parent, Qt::Dialog)
    , ui(new Ui::PrintingConfigDialog)
{
    mSupportingPrinters= QPrinterInfo::availablePrinters();
    mCurrentPrinterIdx = 0;
    mCurrentModuleSNIdx = 0;
    //mMapModule.clear();
    mAppId = "";

    ui->setupUi(this);
    this->setWindowTitle("打印设置");

    connect(ui->button_OK, SIGNAL(released()), this,SLOT(onOKPressed()));

    connect(ui->button_cancel,   SIGNAL(released()),                this, SLOT(onCancelPressed()));
    connect(ui->comboBox_printer,SIGNAL(currentIndexChanged(int )), this, SLOT(onPrinterChanged(int )));
    connect(ui->comboBox_moduleSN, SIGNAL(currentIndexChanged(int)), this, SLOT(onModuleIdChanged(int)));
}

PrintingConfigDialog::~PrintingConfigDialog()
{
    delete ui;
}

static bool getObjectFromFile(QString filePath, QJsonObject *obj)
{
    if (filePath.isEmpty()) {
        return false;
    }

    QFile loadFile(filePath);
    loadFile.open(QIODevice::ReadWrite);
    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(allData, &jsonError);
    if (document.isNull() || (jsonError.error != QJsonParseError::NoError)) {
        return false;
    }

    if (!document.isObject()) {
        return false;
    } else {
        *obj = document.object();
        return true;
    }
}

static void writeObjToFile(QString filePath, QByteArray content)
{
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    file.open(QIODevice::ReadWrite);
    file.resize(0);
    file.write(content, content.length());
    file.close();

    return;
}

static QByteArray jsonObjToByteArray(QJsonObject obj)
{
    QJsonDocument document;
    document.setObject(obj);
    return (document.toJson(QJsonDocument::Compact));
}

static QString getAppId(QString filePath)
{
    QJsonObject appObj;
    int ret;
    ret = getObjectFromFile(filePath, &appObj);
    if (ret == true) {
        if (appObj.contains("AppId")) {
            return appObj["AppId"].toString();
        }
    }
    return "";
}

static QString getSystemDefaultPrinter(void)
{
    QString printerName = QPrinterInfo::defaultPrinterName();
    if (printerName.isEmpty()) {
        printerName = DLG_DEFAULT_PRINTER_NAME;
    }
    return printerName;
}

void PrintingConfigDialog::onCancelPressed()
{
    QDialog::reject();
}

void PrintingConfigDialog::onOKPressed()
{
    QString appName;
    int result;
    BusinessService tmpbsService;
    appName = tmpbsService.readIniSys("APP", KEY_APP_NAME, DEFAULT_VALUE_APP_NAME);
    appName = appName.toLatin1();
    QMessageBox messageBox(QMessageBox::Warning, appName, tr("确定要保存当前打印设置吗？"), QMessageBox::Yes|QMessageBox::No, nullptr);
    result = messageBox.exec();
    if (result == QMessageBox::Yes) {
        onSaveAllParams();
        this->close();
    } else {
        this->close();
    }
    //QDialog::accept();
}

void PrintingConfigDialog::showEvent(QShowEvent *event)
{
    //mCurrentPrinterIdx = 0;
    //initDefaultPrinters();
    //nitDefaultModuleNumbers();
}

void PrintingConfigDialog::onPrinterChanged(int index)
{
    mCurrentPrinterIdx = index;
    updatePaperSize();
    //updatePaperDirection();
}

void PrintingConfigDialog::onSaveAllParams(void)
{
    QJsonObject printSetObj, subPrintSetObj, obj1;
    QString printPath = QCoreApplication::applicationDirPath() + PRINTER_SET_RELATIVE_PATH;
    getObjectFromFile(printPath, &printSetObj);
    subPrintSetObj.insert("Printer", ui->comboBox_printer->currentText());
    subPrintSetObj.insert("Paper", ui->comboBox_paperSize->currentText());
    subPrintSetObj.insert("Direction", ui->comboBox_paperDirection->currentText());
    subPrintSetObj.insert("MarginUp", ui->lineEdit_valueOfTopMargin->text());
    subPrintSetObj.insert("MarginLeft", ui->lineEdit_valueOfLeftMargin->text());
    subPrintSetObj.insert("MarginRight", ui->lineEdit_valueOfRightMargin->text());
    subPrintSetObj.insert("MarginDown", ui->lineEdit_valueOfbottomMargin->text());
    subPrintSetObj.insert("OffsetX", ui->lineEdit_offsetValueOfHorizontal->text());
    subPrintSetObj.insert("OffsetY", ui->lineEdit_offsetValueOfVertical->text());

    if ((ui->comboBox_moduleSN->currentText()+NODE_MODULE_PRINTER) == NODE_PRINTER) {
        printSetObj[NODE_PRINTER] = subPrintSetObj;
    } else {
        obj1.insert((ui->comboBox_moduleSN->currentText()+NODE_MODULE_PRINTER), subPrintSetObj);
        printSetObj[mAppId] = obj1;
    }
    writeObjToFile(printPath, jsonObjToByteArray(printSetObj));

    QJsonObject billCodeJsonObject, obj2;
    int ret;
    QString moduleSN = ui->comboBox_moduleSN->currentText();;
    QString billPath = QCoreApplication::applicationDirPath() + PRINTER_BILL_CODE_RELATIVE_PATH;
    ret = getObjectFromFile(billPath, &billCodeJsonObject);
    if (ret == true) {
        if (!billCodeJsonObject.contains(mAppId)) {
            ret = false;
        } else {
            if (!billCodeJsonObject[mAppId].toObject().contains(moduleSN)) {
                ret = false;
            }
        }
    }
    if (ret == false) {
        obj2 = billCodeJsonObject[mAppId].toObject();
        obj2.insert(moduleSN, moduleSN);
        billCodeJsonObject[mAppId] = obj2;
        writeObjToFile(billPath, jsonObjToByteArray(billCodeJsonObject));
    }
}

void PrintingConfigDialog::initDefaultModuleNumbers()
{
    bool ret;
    QJsonObject billCodeJsonObject, billCodeJsonObjectData;
    QString billPath = QCoreApplication::applicationDirPath() + PRINTER_BILL_CODE_RELATIVE_PATH;
    ret = getObjectFromFile(billPath, &billCodeJsonObject);
    if ((ret == true) && (!mAppId.isEmpty())) {
        if (billCodeJsonObject.contains(mAppId)) {
            billCodeJsonObjectData = billCodeJsonObject[mAppId].toObject();
        } else {
            ret = false;
        }
    }
    mModuleNumbers.clear();
    mModuleName.clear();
    mModuleNumbers.append(DLG_DEFAULT_MODULE_ID);
    mModuleName.append(DLG_DEFAULT_MODULE_ID);
    //mMapModule.clear();
    //mMapModule.insert(DLG_DEFAULT_MODULE_ID, DLG_DEFAULT_MODULE_ID);
    if (ret == true) {
        QJsonObject::const_iterator iter = billCodeJsonObjectData.constBegin();
        while (iter != billCodeJsonObjectData.constEnd()) {
            if (iter.key() == DLG_DEFAULT_MODULE_ID) {
                iter++;
                continue;
            } else {
                if (iter.value().toString().isEmpty()) {
                    mModuleNumbers.append(iter.key());
                    mModuleName.append(iter.key());
                    //mMapModule.insert(iter.key(), iter.key());
                } else {
                    mModuleNumbers.append(iter.key());
                    mModuleName.append(iter.value().toString());
                    //mMapModule.insert(iter.key(), iter.value().toString());
                }
            }
            iter++;
        }
    }

    ui->comboBox_moduleSN->clear();
    ui->comboBox_moduleSN->addItems(mModuleNumbers);
    mCurrentModuleSNIdx = 0;
    ui->comboBox_moduleSN->setCurrentIndex(mCurrentModuleSNIdx);
    ui->label_valueOfModuleName->setText(mModuleName.at(mCurrentModuleSNIdx));
}

void PrintingConfigDialog::initDefaultPrinters()
{
    ui->comboBox_printer->clear();
    ui->comboBox_printer->setMaxCount(mSupportingPrinters.size());

    foreach (QPrinterInfo _info, mSupportingPrinters)
    {
        if (_info.printerName() != DLG_DEFAULT_PRINTER_NAME) {
            mPrinters.append(_info.printerName());
        }
    }

    mPrinters.append(DLG_DEFAULT_PRINTER_NAME);
    if(!mPrinters.isEmpty())
        ui->comboBox_printer->addItems(mPrinters);
}

void PrintingConfigDialog::updatePaperSize()
{
    if(mSupportingPrinters.isEmpty()) return;

    QList<QPageSize> _info = mSupportingPrinters[mCurrentPrinterIdx].supportedPageSizes();

    ui->comboBox_paperSize->clear();
    if (_info.size() > 0) {
        ui->comboBox_paperSize->setMaxCount(_info.size());
    } else {
        return;
    }

    foreach (QPageSize _size, _info)
    {
        ui->comboBox_paperSize->addItem(_size.name());
    }
}

void PrintingConfigDialog::updatePaperDirection()
{
    const QVector<QString> _directions={
        tr("纵向"),
        tr("横向")
    };

    ui->comboBox_paperDirection->clear();
    ui->comboBox_paperDirection->setMaxCount(_directions.size());

    foreach (QString _direction, _directions)
    {
        ui->comboBox_paperDirection->addItem(_direction);
    }
}

void PrintingConfigDialog::updatePrinterParams(void)
{
    QString moduleId = ui->comboBox_moduleSN->currentText();
    QString filePath = QCoreApplication::applicationDirPath() + PRINTER_SET_RELATIVE_PATH;
    bool ret;
    QJsonObject printSetObject, subPrintSetObject, obj1;
    ret = getObjectFromFile(filePath, &printSetObject);
    if (ret == true) {
        if ((moduleId+DLG_NODE_MODULE_PRINTER) == DLG_NODE_PRINTER) {
            subPrintSetObject = printSetObject[DLG_NODE_PRINTER].toObject();
        } else {
            if (mAppId.isEmpty()) {
                ret = false;
            } else {
                obj1 = printSetObject[mAppId].toObject();
                subPrintSetObject = obj1[moduleId+DLG_NODE_MODULE_PRINTER].toObject();
            }
        }
    }

    if (subPrintSetObject.isEmpty() || subPrintSetObject["Printer"].toString().isEmpty()) {
        ui->comboBox_printer->setCurrentText(getSystemDefaultPrinter());
    } else {
        ui->comboBox_printer->setCurrentText(subPrintSetObject["Printer"].toString());
    }

    if (subPrintSetObject.isEmpty() || subPrintSetObject["Paper"].toString().isEmpty()) {
        ui->comboBox_paperSize->setCurrentText(DLG_DEFAULT_PAPER_SIZE);
    } else {
        ui->comboBox_paperSize->setCurrentText(subPrintSetObject["Paper"].toString());
    }

    if (subPrintSetObject.isEmpty() || subPrintSetObject["Direction"].toString().isEmpty()) {
        ui->comboBox_paperDirection->setCurrentText(DLG_DEFAULT_PAPER_ORIENTATION);
    } else {
        ui->comboBox_paperDirection->setCurrentText(subPrintSetObject["Direction"].toString());
    }

    if (subPrintSetObject.isEmpty() || subPrintSetObject["MarginUp"].toString().isEmpty()) {
        ui->lineEdit_valueOfTopMargin->setText(DLG_DEFAULT_MODULE_MARGIN);
    } else {
        ui->lineEdit_valueOfTopMargin->setText(subPrintSetObject["MarginUp"].toString());
    }

    if (subPrintSetObject.isEmpty() || subPrintSetObject["MarginDown"].toString().isEmpty()) {
        ui->lineEdit_valueOfbottomMargin->setText(DLG_DEFAULT_MODULE_MARGIN);
    } else {
        ui->lineEdit_valueOfbottomMargin->setText(subPrintSetObject["MarginDown"].toString());
    }

    if (subPrintSetObject.isEmpty() || subPrintSetObject["MarginLeft"].toString().isEmpty()) {
        ui->lineEdit_valueOfLeftMargin->setText(DLG_DEFAULT_MODULE_MARGIN);
    } else {
        ui->lineEdit_valueOfLeftMargin->setText(subPrintSetObject["MarginLeft"].toString());
    }

    if (subPrintSetObject.isEmpty() || subPrintSetObject["MarginRight"].toString().isEmpty()) {
        ui->lineEdit_valueOfRightMargin->setText(DLG_DEFAULT_MODULE_MARGIN);
    } else {
        ui->lineEdit_valueOfRightMargin->setText(subPrintSetObject["MarginRight"].toString());
    }

    if (subPrintSetObject.isEmpty() || subPrintSetObject["OffsetX"].toString().isEmpty()) {
        ui->lineEdit_offsetValueOfHorizontal->setText(DLG_DEFAULT_PAPER_OFFSET);
    } else {
        ui->lineEdit_offsetValueOfHorizontal->setText(subPrintSetObject["OffsetX"].toString());
    }

    if (subPrintSetObject.isEmpty() || subPrintSetObject["OffsetY"].toString().isEmpty()) {
        ui->lineEdit_offsetValueOfVertical->setText(DLG_DEFAULT_PAPER_OFFSET);
    } else {
        ui->lineEdit_offsetValueOfVertical->setText(subPrintSetObject["OffsetY"].toString());
    }
}

void PrintingConfigDialog::onModuleIdChanged(int index)
{
    mCurrentModuleSNIdx = index;
    ui->label_valueOfModuleName->setText(mModuleName.at(mCurrentModuleSNIdx));
    updatePrinterParams();
}

void PrintingConfigDialog::initPrinterDialog(void)
{
    initDefaultPrinters();
    updatePaperSize();
    updatePaperDirection();
    mAppId = getAppId(QCoreApplication::applicationDirPath() + APPLICATION_SAVE_RELATIVE_PATH);
    initDefaultModuleNumbers();
    updatePrinterParams();
}
