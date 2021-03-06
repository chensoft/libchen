/**
 * Created by Jian Chen
 * Inspired by the book: C++ Concurrency In Action
 * Thanks to its author: Anthony Williams
 * @since  2016.05.15
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 * @thread-safe
 */
#pragma once

#include "chen/mt/semaphore.hpp"
#include "chen/mt/callable.hpp"
#include <functional>
#include <future>
#include <thread>
#include <atomic>
#include <queue>

namespace chen
{
    class threadpool final
    {
    public:
        /**
         * Construct with thread count
         */
        explicit threadpool(std::size_t count = 0, bool fast = false);
        ~threadpool();

    public:
        /**
         * Post a function to pool
         */
        template <typename F>
        std::future<typename std::result_of<F()>::type> post(F f)
        {
            typedef typename std::result_of<F()>::type result_type;

            std::packaged_task<result_type()> task(std::move(f));
            std::future<result_type> future(task.get_future());

            std::unique_lock<std::mutex> lock(this->_mutex);
            this->_queue.emplace(std::move(task));
            lock.unlock();

            this->_semaphore.post();

            return future;
        }

        /**
         * Threads count
         */
        std::size_t count() const;

    private:
        /**
         * Working thread
         */
        void run();

        /**
         * Clean pool
         */
        void clean();

    private:
        std::mutex _mutex;
        std::queue<callable> _queue;

        std::atomic<bool> _exit;
        std::atomic<bool> _fast;
        std::vector<std::thread> _pool;

        semaphore _semaphore;

    public:
        threadpool(const threadpool&) = delete;
        threadpool& operator=(const threadpool&) = delete;
    };
}