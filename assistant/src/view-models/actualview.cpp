#include "actualview.h"
#include <QDebug>
#include <QDateTime>
#include <QDebug>
#include <QEventLoop>
#include <QThread>
#include <QWidget>
#include <QMutexLocker>
#include <QMutex>
#include <QCoreApplication>

#include <QVariantMap>

//#include "logger.h"


void ActualView::setRequires(const QVariantMap &require)
{
    mRuntimeData = require;
}

void ActualView::notifyViewComplete()
{
    emit sigIsCompleted();
}

void ActualView::notifyIsError()
{
    emit sigIsError();
}

void ActualView::onViewShowing()
{
    show();
    QWidget::raise();
    emit sigShowing();
}

void ActualView::onViewHidden()
{
    hide();
    emit sigHidden();
}

void ActualView::onViewOpen()
{
    emit sigOpenning();
}

void ActualView::onViewClose()
{
    if(!mRuntimeData.isEmpty()) mRuntimeData.clear();

    emit sigClosing();
}

void ActualView::onViewRefesh()
{
    emit sigRefeshing();
}


// return QVariantMapVisitor::getVisitor(mRuntimeData);
