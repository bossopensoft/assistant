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


#ifndef CACONFIGMANAGER_H
#define CACONFIGMANAGER_H

#include "iconfighandler.h"

/*
    #吉大正元ca
    jit=TCA4Jit
    #北京ca
    bjca=TCA4BJCA
    #宁夏海泰ca
    cwca=TCA4CWCA
    #辽宁嘉鸿ca
    lnjh=TCA4LNJHKey
    #凯特信安ca
    kinsec=TCA4Kinsec
    #天津ca
    tjca=TCA4TJCA
    #吉大-飞天诚信ca
    ftcx=TCA4JitFTCX
    #山东ca
    sdca=TCA4SDCA
    #福建ca
    fjca=TCA4FJCA
    #格尔ca
    koal=TCA4KOAL
    #广东ca
    gdca=TCA4GDCA
    #天津-中国金融ca
    cfca=TCA4CFCA
*/
class IniFileHandler;
class ConfigManager;

class CAConfigManager : public IConfigHandler
{
public:

    CAConfigManager();
    CAConfigManager(const CAConfigManager& other);

    Content &getClass();
    QString filePath()const;

    bool    save() override;
    bool    load() override;

    CAConfigManager& operator=(const CAConfigManager& other)=delete; //disable operator=
private:

    CAConfigManager(IniFileHandler* fileHandler);
    Content            mClassContent;
    IniFileHandler*    mFileHandler;

friend class ConfigManager;
};

#endif // CACONFIGMANAGER_H
