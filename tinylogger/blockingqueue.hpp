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


#ifndef _BLOCKINGQUEUE_H
#define _BLOCKINGQUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <iostream>
#include "tracer.h"

namespace TINY_LOGGER_PRIVATE {

//#define _DEBUG_Q
#ifdef _DEBUG_Q
#define LOG_Q(...) TRACE(__VA_ARGS__)
#else
#define LOG_Q(...)
#endif

template<typename T>
class BlockingQueue
{
private:
    std::mutex mutex_;
    std::condition_variable conditionPush_;
    std::condition_variable conditionPop_;
    std::deque<T> queue_;
    bool isBlocking_ = true;

    std::uint16_t maxSize_ = 3;


    T pop_blockOnEmpty()
    {
        T val;
//        LOG("");
        {
            std::unique_lock<std::mutex> lock(this->mutex_);
            LOG_Q("obtained lock");
            this->conditionPop_.wait(lock, [=] {
                LOG_Q("Checking wait condition");
                if (isBlocking_)
                {
                    LOG_Q("isBlocking == true");
                }
                else
                {
                    LOG_Q("isBlocking == false");
                }
                if (this->queue_.empty())
                {
                    LOG_Q("queue.empty() == true");
                }
                else
                {
                    LOG_Q("queue.empty() == false");
                }
                return !isBlocking_ || !this->queue_.empty();
            });

            if (!this->queue_.empty())
            {
                LOG_Q("queue contains data, popping...");
                val = std::move(this->queue_.back());
                this->queue_.pop_back();
                LOG_Q("data popped!");
//                LOG_Q(val);
            }
            else
            {
                LOG_Q("queue empty, leaving...");
            }
        }
        this->conditionPush_.notify_one();
        LOG_Q("return");
        return val;
    }

public:
    BlockingQueue()
            : mutex_()
            , conditionPush_()
            , conditionPop_()
            , queue_()
    {}


    void push(T const &value)
    {
//        LOG_Q(value);
        {
            std::unique_lock<std::mutex> lock(this->mutex_);
            LOG_Q("obtained lock");

            this->conditionPush_.wait(lock, [=] { return !isBlocking_ || this->queue_.size() < maxSize_; });
            LOG_Q("queue has room, pushing...");
//            LOG_Q(value);
            queue_.push_front(value);
        }
        this->conditionPop_.notify_one();
        LOG_Q("return");
    }


    T pop(bool blockOnEmpty = true)
    {
        if (blockOnEmpty)
        {
            return pop_blockOnEmpty();
        }

        std::unique_lock<std::mutex> lock(this->mutex_);
        T val = std::move(this->queue_.back());
        this->conditionPush_.notify_one();
        return val;
    }


    void setBlocking(bool isBlocking)
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        isBlocking_ = isBlocking;

        if (isBlocking)
        {
            LOG_Q("Blocking set to true");
        }
        else
        {
            LOG_Q("Blocking set to false");
            this->conditionPush_.notify_all();
            this->conditionPop_.notify_all();
        }
    }

    bool isBlocking() const
    {
        return isBlocking_;
    }


    void setMaxSize(const std::uint32_t maxSize)
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->maxSize_ = maxSize;
    }


    bool isEmpty()
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        return this->queue_.empty();
    }
};

}

#endif //_BLOCKINGQUEUE_H
