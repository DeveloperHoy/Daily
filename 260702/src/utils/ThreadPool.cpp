#include "ThreadPool.h"

namespace ai::utils {

ThreadPool::ThreadPool(size_t threadCount) {
    for (size_t i = 0; i < threadCount; ++i) {
        m_workers.emplace_back([this]() { this->worker(); });
    }
}

ThreadPool::~ThreadPool() {
    // Signal workers to stop
    // Wait for workers to finish
    m_tasks.emplace([](){}); // dummy task to wake up workers
    // join
}

template<typename F, typename... Args>
void ThreadPool::enqueue(F&& f, Args&&... args) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_tasks.emplace(std::forward<F>(f), std::forward<Args>(args)...);
    }
    m_cond.notify_one();
}

void ThreadPool::run() {
    while (true) {
        std::function<void()> task;
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            if (m_tasks.empty()) {
                return; // Exit if empty (or wait)
            }
            task = std::move(m_tasks.front());
            m_tasks.pop();
        }
        task();
    }
}

void ThreadPool::worker() {
    while (true) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_tasks.empty()) {
                m_cond.wait(lock);
            }
        }
    }
}

} // namespace ai::utils
