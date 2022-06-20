//
// Created by pc on 17-11-21.
//

#ifndef H_APP_THR_THREADPOOL_H
#define H_APP_THR_THREADPOOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace app::thr {

    class ThreadPool {
        friend void run(ThreadPool& pool, std::atomic<bool>& ready_status);

      public:
        explicit ThreadPool(size_t num_cores);
        ~ThreadPool();

        void               resize(size_t num_cores);
        void               stop_all();
        void               add(std::function<void()>&& function);
        void               wait();
        [[nodiscard]] bool all_ready() const;

        [[nodiscard]] static ThreadPool& get();

      private:
        std::atomic<bool>                 m_stop{false};
        mutable std::mutex                m_queue_mutex;
        mutable std::mutex                m_task_done_mutex;
        std::condition_variable           m_task_condition_variable;
        std::condition_variable           m_task_done_condition_variable;
        std::vector<std::thread>          m_threads;
        std::deque<std::atomic<bool>>     m_ready_statuses;
        std::queue<std::function<void()>> m_function_queue;

        static std::unique_ptr<ThreadPool> s_thread_pool;
    };
} // namespace app::thr

#endif // H_APP_THR_THREADPOOL_H
