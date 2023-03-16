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


#ifndef FILELIST_H
#define FILELIST_H

#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <cstdio>

namespace TINY_LOGGER_PRIVATE {
typedef  std::list<std::string> FileList;
typedef std::function<bool(const std::string&, const std::string&)> Comparerr;


class FileListSorter
{
private:
    std::vector<std::string> mSortingCandidators;
    Comparerr  mComparer;

public:
    FileListSorter(const FileList& _files, Comparerr comparer):mComparer(comparer)
    {
        for(auto _f: _files)
        {
            mSortingCandidators.push_back(_f);
        }
    }

    void Sort()
    {
        std::sort(mSortingCandidators.begin(), mSortingCandidators.end(), mComparer);
    }

    FileList getResult()const
    {
        FileList _duplist;
        for(auto _f: mSortingCandidators)
        {
            _duplist.push_back(_f);
        }

        return _duplist;
    }

    void dumpResult()
    {
        printf("dumpResult() \n");
        for_each(mSortingCandidators.begin(),
                 mSortingCandidators.end(),
                 [](const std::string &_string)
                 {
                     printf("file=%s \n", _string.c_str());
                 });
    }
};

}


#endif //
