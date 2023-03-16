#include "aboutdialog.h"
#include <QCloseEvent>
#include <QList>
#include <QStandardItemModel>
#include <QTextCodec>


#include "ui_aboutdialog.h"
#include "sysconfigmanager.h"

#include "../controllers/businessservice.h"
const char DATE[]={__DATE__};
const char TIME[]={__TIME__};

static QString __toReadableString(const QString& data)
{
   QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
   QTextCodec* gbk  = QTextCodec::codecForName("gbk");

   QString _unicodeStr  = gbk->toUnicode(data.toLocal8Bit());
   QByteArray utf8_bytes=utf8->fromUnicode(_unicodeStr);

   return QString::fromLatin1(utf8_bytes);
}

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    this->Init();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}


void AboutDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}

void AboutDialog::on_pushButton_clicked(bool checked)
{
    Q_UNUSED(checked);
    this->hide();
}

void AboutDialog::Init()
{
    SysConfigManager mng;

    if (!mng.load())
    {
        return;
    }

    Content appInfo = mng.getAppInfo();
    QString _buildDateTime = QString("\n(%1-%2)").arg(DATE).arg(TIME);

    QString label = QString(appInfo.value(KEY_APP_NAME).toString().toLatin1()) + appInfo.value(KEY_APP_VERSION).toString() + _buildDateTime;

    ui->clientNameLabel->setText(label);

    Content dllInfo = mng.getSubmodulesInfo();
    QList<QString> keys = dllInfo.keys();
    m_model = new QStandardItemModel(ui->tableView);

    int row = 0;
    for (QString key : keys) {
        QString val = dllInfo.value(key).toString();
        QList<QStandardItem*> items;
        items.append(new QStandardItem(key));
        items.append(new QStandardItem(val));
        m_model->appendRow(items);
        row++;
    }

    ui->tableView->setModel(m_model);
    ui->tableView->setColumnWidth(0, 180);

    ui->tableView->setModel(m_model);
}
