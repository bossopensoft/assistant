#ifndef ACTUALVIEWMODEL_H
#define ACTUALVIEWMODEL_H

#include "actualview.h"
#include <QMap>

class ActualView;

// the concrete view model was inherited from Actual View Model.
class ActualViewModel : public QObject
{
    Q_OBJECT
public:
    ActualViewModel(QObject* parent=0):QObject(parent){}

    ~ActualViewModel(){
        release();
    }

    virtual void bindView(ActualView* view){}
    virtual void unbind(){}

    virtual void prepareRuntimeData(const QVariantMap& runtimeData)
    {
        mRuntimeData = runtimeData;
    }

    virtual QVariantMap& getRuntimeData()
    {
        return mRuntimeData;
    }

    virtual void    setViewEventObserver(QObject* parent, IViewModel::ViewEventObserver* observer)
    {
        if(mObservers.contains(parent)) releaseEventObserver(parent);

        mObservers[parent] = observer;
    }

    virtual void      releaseEventObserver(QObject* parent)
    {
        IViewModel::ViewEventObserver* observer = mObservers[parent];
        delete observer;

        mObservers[parent] = nullptr;
        mRuntimeData.clear();
    }


    virtual void updateData(const QVariantMap& runtimeData)
    {
        emit sigViewUpdate(runtimeData);
    }

    virtual void start(){
        emit sigModelStart(mRuntimeData);
    }

    QVariantMap     response()const {return mRuntimeData[RUNTIMEDATA_TAG_RESPONSE].toMap();}
    int             status()const {return mRuntimeData[RUNTIMEDATA_TAG_STATUS].toInt();}

public slots:
    void onCompleted(const QVariantMap& runtimeData){
         mRuntimeData = runtimeData;

         emit sigCompleted(mRuntimeData);
    }

    void onError(int error, const QString& messag){
         mError = error;
         mErrorMessage = messag;

         emit sigIsError(mError, mErrorMessage);
    }

signals:
    void sigViewUpdate(const QVariantMap& );
    void sigModelStart(const QVariantMap& );
    void sigCompleted(const QVariantMap& );
    void sigIsError(int, const QString& );

protected:
    inline void setSuccessfulStatus(int _status=0)
    {
        setResponse(_status, {});
    }

    void setResponse(int status, const QVariantMap& response={}){
        mRuntimeData[RUNTIMEDATA_TAG_STATUS]   = status;

        if(!response.isEmpty()) mRuntimeData[RUNTIMEDATA_TAG_RESPONSE] = response;
    }

    void setErrorStatus(int status){
        mRuntimeData[RUNTIMEDATA_TAG_STATUS]   = status;
    }

    virtual void  release() // calling when model was destory by system.
    {
        foreach (IViewModel::ViewEventObserver* observer, mObservers.values()) {
            delete observer;
        }

        mObservers.clear();
        if(!mRuntimeData.isEmpty()) mRuntimeData.clear();
    }

private:


    QVariantMap                                  mRuntimeData;
    int                                                  mError;
    QString                                          mErrorMessage;
    QMap<QObject* , IViewModel::ViewEventObserver*>  mObservers;
};


#endif /// ACTUALVIEWMODEL_H
