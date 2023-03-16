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

#ifndef STRINGSPITE_H
#define STRINGSPITE_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <tuple>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <algorithm>
#include <time.h>


namespace TinyLogger_StringUtility
{
    static std::vector<std::string> split(const std::string& str, char delim)
    {
        std::stringstream ss(str);
        std::string item;
        std::vector<std::string> elems;

        while(std::getline(ss, item, delim))
        {
            elems.push_back(std::move(item));
        }

        return elems;
    }



    static std::string getLocalTimeInString()
    {
        char strDateTime[25+1]={0};

        auto now     = std::chrono::system_clock::now();
        time_t tt    = std::chrono::system_clock::to_time_t(now);

        struct tm time_tm={0};
        localtime_r(&tt, &time_tm);

        clock_t _tick= clock();




        snprintf(strDateTime, sizeof(strDateTime) -1,"%d-%02d-%02d-%d-%d-%d-%ld",
                                                     time_tm.tm_year+1900, time_tm.tm_mon+1, time_tm.tm_mday,
                                                     time_tm.tm_hour, time_tm.tm_min, time_tm.tm_sec,(long)(_tick));

        std::string _sDateTime;
        _sDateTime.assign(strDateTime);
        return _sDateTime;
    }

    static bool getDateFromString(struct tm* time_tm, long* _tick,const std::string date, char delim='-')
    {

       sscanf(date.data(), "%d-%02d-%02d-%d-%d-%d-%ld",
                              &time_tm->tm_year, &time_tm->tm_mon, &time_tm->tm_mday,
                              &time_tm->tm_hour, &time_tm->tm_min, &time_tm->tm_sec, _tick);

//       std::vector<std::string> _substrings=  split(date, delim);
//       if(_substrings.size() < 3) return  false;


//       time_tm->tm_year = std::atoi(_substrings[0].c_str());
//       time_tm->tm_mon  = std::atoi(_substrings[1].c_str());
//       time_tm->tm_mday = std::atoi(_substrings[2].c_str());
       return true;
    }

    static bool getDateStructFromFileName(struct tm* time_tm, long* _tick,const std::string _fileName, char delim='_')
    {
        memset(time_tm, 0 ,sizeof(struct tm));

        size_t _found= _fileName.rfind(".");
        if(_found == std::string::npos) return false;

        std::vector<std::string> _sub_strings = split(_fileName.substr(0,_found), delim);
        if(_sub_strings.size() < 2) return false;


        return getDateFromString(time_tm, _tick, _sub_strings[1]);
    }

}


#endif // STRINGSPITE_H
