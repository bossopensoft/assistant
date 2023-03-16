#ifndef ACTUALVIEW_H
#define ACTUALVIEW_H

#include <QWidget>
#include <QDialog>
#include <QKeyEvent>
#include <QVariantMap>
#include <QEventLoop>
#include <QTimer>
#include <QMutex>
#include <QLabel>
#include <QDebug>
#include <QByteArray>
#include <QGraphicsBlurEffect>
#include <QDialog>

#include "logger.h"
#include <QApplication>
#include <QDesktopWidget>

class QEventLoop;
class IContentView;
class IViewDelegate;


class ActualView : public QWidget
{
    Q_OBJECT
public:
    explicit ActualView(QWidget* parent)
      : QWidget(parent, Qt::FramelessWindowHint | Qt::Dialog)
    {

    }

    void resizeEvent(QResizeEvent *e)
    { //move to the center
        Q_UNUSED(e)
//        int nScreenWidth = QApplication::desktop()->screenGeometry().width() - SCREEN_OFFSET_X;
//        int nScreenHeigth = QApplication::desktop()->screenGeometry().height() - SCREEN_OFFSET_Y;
//        move((nScreenWidth - this->width())/2 + SCREEN_OFFSET_X, (nScreenHeigth - height())/2 + SCREEN_OFFSET_Y);
    }

    virtual bool            visiable()const{return QWidget::isVisible();}
    virtual QString         name()const{return QObject::objectName();}

    virtual void setRequires(const QVariantMap& require);
    virtual void notifyViewComplete();
    virtual void notifyIsError();

signals:
    void sigShowing();
    void sigHidden();
    void sigOpenning();
    void sigClosing();
    void sigRefeshing();

    void sigIsCompleted();
    void sigIsError();

public slots:
    /// the following method must define as slot function.
    virtual void        onViewShowing();
    virtual void        onViewHidden();
    virtual void        onViewOpen();
    virtual void        onViewClose();
    virtual void        onViewRefesh();

protected:
    QVariantMap  mRuntimeData;
};



#endif // ACTUALVIEW_H
