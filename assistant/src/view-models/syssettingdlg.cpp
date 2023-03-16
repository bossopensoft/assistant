#include "syssettingdlg.h"
#include "ui_syssettingdlg.h"
#include "businessservice.h"
#include "inifilehandler.h"
#include "configmanager.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include <QValidator>

sysSettingDlg::sysSettingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sysSettingDlg)
{
    ui->setupUi(this);
    this->setWindowTitle("系统设置");
    //this->setWindowFlag(Qt::WindowCloseButtonHint);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    //setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    connect(ui->comfirmButton, SIGNAL(clicked(bool)), this, SLOT(onComfirmBtnClicked()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(onCencelBtnClicked()));
    ui->autoStartCheckBox->setHidden(true);

}

sysSettingDlg::~sysSettingDlg()
{
    delete ui;
}

void sysSettingDlg::initSysSettingDlg(void)
{
    BusinessService tmpbsService;
    appName = tmpbsService.readIniSys("APP", KEY_APP_NAME, DEFAULT_VALUE_APP_NAME);
    appName = appName.toLatin1();
    //appName = "财政票据客户端综合管理组件
    QRegExp rx("^([1-9])|1[0-9]|2[0-4]$");
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    ui->timelineEdit->setValidator(pReg);
    ui->timelineEdit->setText(tmpbsService.readIniSys("APP", KEY_APP_UPDATE_INTERVAL, "8"));
}

void sysSettingDlg::onComfirmBtnClicked(void)
{
    int result;
    //result = QMessageBox::warning(nullptr, appName, tr("确定要保存当前系统设置吗？"),tr("是"),tr("否"), nullptr);
    QMessageBox messageBox(QMessageBox::Warning, appName, tr("确定要保存当前系统设置吗？"), QMessageBox::Yes|QMessageBox::No, nullptr);
    result = messageBox.exec();
    if (result == QMessageBox::Yes) {
        //qDebug()<<"yes";
        onSaveSettingProcess();
        emit resetUpdateTimer(false);
        this->close();
    } else {
        this->close();
    }
    return;
}

void sysSettingDlg::onCencelBtnClicked(void)
{
    int result;
    QMessageBox messageBox(QMessageBox::Warning, appName, tr("确定要保存当前系统设置吗？"), QMessageBox::Yes|QMessageBox::No, nullptr);
    result = messageBox.exec();
    if (result == QMessageBox::Yes) {
        onSaveSettingProcess();
        this->close();
    } else {
        this->close();
    }
    return;
}

void sysSettingDlg::onSaveSettingProcess(void)
{
    QString _filePath =ConfigManager::getInstance()->getSysConfigManager()->filePath();
    QSharedPointer<IniFileHandler> tmpFileHandler(new IniFileHandler(_filePath));
    if (tmpFileHandler->exist()) {
        tmpFileHandler->setValue("/APP/application.upgrade.timer.interval", ui->timelineEdit->text());
    }

    return;
}
