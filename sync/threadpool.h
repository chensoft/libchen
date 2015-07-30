/**
 * Created by Jian Chen
 * @since  2015.07.30
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://www.chensoft.com
 */
#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include "semaphore.h"

namespace chen
{
    // todo add unit test
    // todo maybe use template in future
    class threadpool final
    {
    public:
        typedef std::function<void ()> type_func;

    public:
        threadpool(unsigned int count = 0);
        ~threadpool();

    public:
        // todo maybe return std::future in future
        /**
         * Put a job into queue
         */
        void async(const type_func &job);

    protected:
        void run();

    private:
        bool _destroy = false;  // is wait for destroy

        std::mutex _mutex;              // queue lock
        std::vector<type_func> _queue;  // queue job

        std::vector<std::thread> _pool;  // thread
        chen::semaphore _semaphore;      // semaphore
    };
}