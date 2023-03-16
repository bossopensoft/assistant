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

#ifndef TRACER_H
#define TRACER_H

#include <stdarg.h>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <sstream>

namespace TINY_LOGGER_PRIVATE {


#define _DEBUG

#ifdef _DEBUG
#define TRACE(...)      Tracer::trace((ulong)__LINE__,__FUNCTION__, ##__VA_ARGS__);
#define TRACE_VA(...)   Tracer::trace((ulong)__LINE__,__FUNCTION__, Tracer::formating(__VA_ARGS__));
#define TRACE_E(...)    Tracer::trace((ulong)__LINE__,__FUNCTION__, ##__VA_ARGS__);
#else
#define LOG(...)
#define LOGVA(...)
#define LOG_E(...)
#endif

#define MAX_BUFFER_SIZE (5*1024+1)

class Tracer
{
private:
    static std::mutex printMutex_;
    static char vprintBuffer[MAX_BUFFER_SIZE];
public:
//    static void log(const std::string &msg=std::string(), const std::string func=__FUNCTION__,ulong line=(ulong)__LINE__)
//    {
//        std::unique_lock<std::mutex> lock(Logger::printMutex_);
//        std::cout << "[@ln="<<line<<":"<<func << "[tid: " << std::this_thread::get_id() << "] " << msg
//                  << std::endl;
//    }

    static std::string formating(const char* fmt,...)
    {
          char vprintBuffer[MAX_BUFFER_SIZE]={0};
          va_list _va;
          va_start(_va, fmt);
          vsnprintf(&vprintBuffer[0], sizeof(vprintBuffer)-1, fmt, _va);
          va_end(_va);

          std::string _dup;
          return _dup.assign(&vprintBuffer[0]);
    }

    static void trace(ulong line, const std::string func, const std::string &msg)
    {
        std::unique_lock<std::mutex> lock(Tracer::printMutex_);
        std::cout << "[@ln="<<line<<":"<<func << " - tid: " << std::this_thread::get_id() << "] " << msg
                  << std::endl;
    }
};

}

#endif // TRACER_H
