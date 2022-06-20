//
// Created by pc on 17-11-21.
//

#include "ThreadPool.h"

#include "ThreadSettings.h"

#include <algorithm>

namespace app::thr {

    std::unique_ptr<ThreadPool> ThreadPool::s_thread_pool;

    void run(ThreadPool& pool, std::atomic<bool>& ready_status);

    ThreadPool::ThreadPool(size_t num_cores) {
        resize(num_cores);
    }

    ThreadPool::~ThreadPool() {
        stop_all();
    }

    void ThreadPool::resize(size_t num_cores) {
        stop_all();
        m_stop = false;
        m_threads.clear();
        m_threads.reserve(num_cores);
        m_ready_statuses.resize(num_cores);
        for (size_t i = 0; i != num_cores; ++i) {
            m_ready_statuses[i] = true;
            m_threads.emplace_back(run, std::ref(*this), std::ref(m_ready_statuses[i]));
        }
    }

    void ThreadPool::stop_all() {
        m_stop = true;
        m_task_condition_variable.notify_all();
        std::for_each(m_threads.begin(), m_threads.end(), std::mem_fn(&std::thread::join));
    }

    void ThreadPool::add(std::function<void()>&& function) {
        std::lock_guard<std::mutex> lock(m_queue_mutex);
        m_function_queue.emplace(std::move(function));
        m_task_condition_variable.notify_one();
    }

    void ThreadPool::wait() {
        std::unique_lock<std::mutex> lock(m_task_done_mutex);
        m_task_done_condition_variable.wait(lock, [this]() { return all_ready(); });
    }

    void run(ThreadPool& pool, std::atomic<bool>& ready_status) {
        ready_status = true;
        while (true) {
            std::function<void()> f;
            {
                std::unique_lock<std::mutex> lock(pool.m_queue_mutex);
                pool.m_task_condition_variable.wait(lock, [&] { return (not pool.m_function_queue.empty()) || pool.m_stop; });
                if (pool.m_stop) // Stop even if there are tasks left.
                    break;
                else {
                    ready_status = false;
                    f            = pool.m_function_queue.front();
                    pool.m_function_queue.pop();
                }
            }
            f();
            ready_status = true;
            std::unique_lock<std::mutex> lock(pool.m_task_done_mutex);
            pool.m_task_done_condition_variable.notify_all();
        }
        ready_status = true;
    }

    bool ThreadPool::all_ready() const {
        std::lock_guard<std::mutex> lock(m_queue_mutex);
        return std::all_of(m_ready_statuses.begin(), m_ready_statuses.end(), [](const std::atomic<bool>& r) { return static_cast<bool>(r); }) && m_function_queue.empty();
    }

    ThreadPool& ThreadPool::get() {
        if (not s_thread_pool)
            s_thread_pool = std::make_unique<ThreadPool>(g_thread_count);

        return *s_thread_pool;
    }

} // namespace app::thr