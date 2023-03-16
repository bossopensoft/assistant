/***************************************************************************
 *   This file is part of the Bosssoft Assistant Client Project                          *
 *   Copyright (C) 2023 by Bin Yan                                                                *
 *   yanbin@bosssoft.com.cn                                                                        *
 *                                                                                                                   *
 **                   GNU General Public License Usage                                      **
 *                                                                                                                  *
 *   This library is free software: you can redistribute it and/or modify        *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or          *
 *   (at your option) any later version.                                                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                                                                   *
 **                  GNU Lesser General Public License                                      **
 *                                                                                                                   *
 *   This library is free software: you can redistribute it and/or modify         *
 *   it under the terms of the GNU Lesser General Public License as             *
 *   published by the Free Software Foundation, either version 3 of the       *
 *   License, or (at your option) any later version.                                         *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with this library.                                                                *
 *   If not, see <http://www.gnu.org/licenses/>.                                            *
 *                                                                                                                   *
 *   This library is distributed in the hope that it will be useful,                     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the   *
 *   GNU General Public License for more details.                                          *
 ****************************************************************************/

#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <QString>

#include <QMutex>
#include <QMutexLocker>
#include "httprequesthandler.h"

using namespace stefanfrings;

class IController
{
public:
    virtual ~IController(){}

    virtual QString name()const =0;
    virtual bool    service(HttpRequest& request, HttpResponse& response)=0;
};


class ControllerDecorator : public IController
{
public:
    ControllerDecorator(IController* controller):mBusy(false),mParentController(controller){}

    virtual ~ControllerDecorator()
    {
        if(mParentController) delete  mParentController;
    }

    virtual bool service(HttpRequest &request, HttpResponse &response) override
    {
       StatusGuard _guard(&mBusy);
       //QMutexLocker _autoLocker(&mMutex);

       return mParentController->service(request, response);
    }

    virtual QString name()const override{return mParentController->name();}

    bool isBusy()const{return mBusy;}

private:
    class StatusGuard
    {
    public:
        StatusGuard(volatile bool* status):mStatus(status){*mStatus=true;}
        ~StatusGuard(){*mStatus = false;}
    private:
        volatile bool* mStatus;
    };

    volatile bool         mBusy;
    IController*          mParentController;
    QMutex                mMutex;
};

#endif // ISERVICE_H
