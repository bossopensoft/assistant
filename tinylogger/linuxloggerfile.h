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

#ifndef LINUXLOGGERFILE_H
#define LINUXLOGGERFILE_H

#include "iloggerfilehandler.h"
#include "filelist.h"
#include "stringutility.h"

#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include "tracer.h"

static inline void __nop(){}

#define _DEBUG_LINUX_LOG_HANDER
#ifdef _DEBUG_LINUX_LOG_HANDER
#define LOG_LINUX_LOGGER(...)    TRACE(__VA_ARGS__)
#define LOG_LINUX_LOGGER_VA(...) TRACE_VA(__VA_ARGS__)
#define LOG_LINUX_LOGGER_E(...)  TRACE_E(__VA_ARGS__)

#define LOG_LINUX_TRACE(EXPRESSION, ...)      \
        do{                                   \
            TRACE_VA(__VA_ARGS__);                                                         \
            if(!(EXPRESSION)) TRACE_VA("%s was faiulre!, reason=%m",#EXPRESSION);          \
        }while(0)

#else
#define LOG_LINUX_LOGGER(...)
#define LOG_LINUX_LOGGER_VA(...)
#define LOG_LINUX_LOGGER_E(...)
#endif

namespace TINY_LOGGER_PRIVATE {

class LinuxLoggerFile : public ILoggerFileHandler
{
private:
    int fd_ =-1;
public:
    LinuxLoggerFile()
    {

    }

    bool OpenLogfile(const std::string& fileName) override
    {
      fd_=-1;

      fd_ = open(fileName.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666);
      LOG_LINUX_TRACE(fd_ >0,"open file=%s, fd=%d",fileName.c_str(), fd_);

      if (fd_ == -1) return false;
      return true;  // Everything worked
    }

    int WriteLog(const std::string logText) override
    {
        if(fd_ == -1)
        {
            LOG_LINUX_LOGGER_E("invalid fd");
            return -1;
        }

       int _write_size= write(fd_, logText.c_str(), logText.size());
       fsync(fd_);
       LOG_LINUX_TRACE(_write_size > 0, "write file fd=%d, _write_size=%d",fd_, _write_size);
       return _write_size;
    }

    void CloseLogFile() override
    {
        LOG_LINUX_LOGGER("close log file");
        if(fd_ == -1) return;
        fsync(fd_);
        close(fd_);

        fd_ = -1;
    }

    bool RemoveFile(const std::string& fileName) override
    {
       bool _isOk= (remove(fileName.c_str()) == 0?true:false);
       LOG_LINUX_TRACE(_isOk,"remove file=%s ",fileName.c_str());
       return _isOk;
    }

    bool MakeLoggerWorkSpace(const std::string& dirName) override
    {
       int _r = mkdir(dirName.c_str(), 0777);
       LOG_LINUX_TRACE(_r > 0,"mkdir file=%s _r=%d",dirName.c_str(),_r);
       return _r < 0?false:true;
    }

    bool FileExist(const std::string& file) override
    {
        bool _is_exist= (0==access(file.c_str(), F_OK))?true:false;
        LOG_LINUX_LOGGER_VA("file=%s is exist=%s", file.c_str(), (_is_exist?"true":"false"));
        return _is_exist;
    }

    bool IsVisiable(const std::string& workspace) override
    {
        return (0==access(workspace.c_str(), R_OK | W_OK))?true:false;
    }


//    bool TravelDirectory(FileList& files,
//                         const std::string& dirname,
//                         const std::string& fileExtension,
//                         int deepth) override

    bool TravelDirectory(FileList& files,
                         const std::string& dirname,
                         const std::string& fileExtension,
                         std::function<bool(const std::string&, const std::string& )> comparer,
                         int deepth) override
    {
        (void)(deepth);

        DIR *ptDir;
        struct dirent *ent;

        FileList _condidateList;

        ptDir = opendir(dirname.c_str());
        if(ptDir == NULL) return false;

        while((ent = readdir(ptDir)) != NULL)
        {
            if(ent->d_type != DT_REG) continue;

            std::string _file_name;
            _file_name.assign(ent->d_name);

            size_t _found = _file_name.rfind(fileExtension);
            if(_found == std::string::npos) continue;

            _condidateList.emplace_back(_file_name);
        }

        /// file name format as: XXXX_DATETIME.log
        FileListSorter sorter(_condidateList, comparer);
        sorter.Sort();

        for(auto _filename : sorter.getResult())
        {
           files.emplace_front(dirname + "/" + _filename);
        }

        closedir(ptDir);
        ptDir = NULL;
        return true;
    }


    size_t GetFileSize(const std::string& filename) override
    {
        struct stat buf;

        if(stat(filename.c_str(), &buf) < 0)
        {
            return 0;
        }

        return buf.st_size;
    }
};

}

#endif // LINUXLOGGERFILE_H
