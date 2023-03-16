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


#ifndef ILOGGERFILEHANDLER_H
#define ILOGGERFILEHANDLER_H

//#include <string>
//#include <list>

//typedef  std::list<std::string> FileList;
#include "filelist.h"
#include <functional>
namespace TINY_LOGGER_PRIVATE {


struct ILoggerFileHandler
{
    virtual ~ILoggerFileHandler(){}
    virtual bool OpenLogfile(const std::string& fileName) =0;
    virtual int WriteLog(const std::string logText)       =0;
    virtual void CloseLogFile()                           =0;

    virtual bool RemoveFile(const std::string& fileName)          = 0;
    virtual bool MakeLoggerWorkSpace(const std::string& dirName)  = 0;

    virtual bool FileExist(const std::string& file)               = 0;
    virtual bool IsVisiable(const std::string& file)              = 0;

    virtual bool TravelDirectory(FileList& files,
                                 const std::string& dirname,
                                 const std::string& fileExtension,
                                 std::function<bool(const std::string&, const std::string& )> compared,
                                 int deepth)                      =0;


    virtual size_t GetFileSize(const std::string& filename)       =0;
};

}

#endif // ILOGGERFILEHANDLER_H
