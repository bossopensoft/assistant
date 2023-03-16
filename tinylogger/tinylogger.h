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

#ifndef TINYLOGGER_H
#define TINYLOGGER_H

#include <string>
#include <vector>
#include <set>
#include <memory>
#include <thread>

struct LoggerFileNode;
struct BlockingQueueWrapper;

class TinyLogger
{
public:

    typedef unsigned int                    loggerlv_t;
    typedef LoggerFileNode*                 LoggerFileNode_Ptr;

    struct LoggerFileManager
    {
       size_t                           capacity;
       size_t                           file_size;
       std::vector<LoggerFileNode_Ptr>  loggerFiles;
    };

    enum LEVEL_VALE
    {
        LV_DEBUG       = 0x00000001,
        LV_INFO        = 0x00000002,
        LV_WARNING     = 0x00000004,
        LV_CRITICAL    = 0x00000008,
        LV_FATAL       = 0x00000010,
    };

    struct Configuration
    {
        std::string Workspace;
        std::string BaseFileName;
        std::string FileNameExtension;
        size_t         MaxFileSize;
        size_t         MaxFileNum;
    };

    TinyLogger();
//    static TinyLogger* getInstance();
    virtual ~TinyLogger();
    void setAvaliableLV(loggerlv_t level){mLevel = level;}
    void setLoggerConfiguration(const Configuration& config);
    bool initialization();

    int writeLogger(loggerlv_t level, std::string message);
private:

    void startHandler();
    int  logger_handler();
    bool createLoggerFile();
    bool getFileName(std::string& fileNameOut);

//    static TinyLogger* mINSTANCE;

    Configuration                 mConfiguration;
    loggerlv_t                    mLevel;
    LoggerFileManager             mLoggerFileManager;
    BlockingQueueWrapper*         mQueue;
    std::thread*                  mBackendThread;
    volatile bool                 mIsBackendThreadRunning;
};

#endif // TINYLOGGER_H
