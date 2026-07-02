#pragma once

#include <vector>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace ai::utils {

class ThreadPool {
public:
    explicit ThreadPool(size_t threadCount);
    ~ThreadPool();

    // Submit a task
    template<typename F, typename... Args>
    void enqueue(F&& f, Args&&... args);

    // Get task for a thread (if needed, or just run)
    void run(); 

private:
    void worker();

    mutable std::mutex m_mutex;
    std::condition_variable m_cond;
    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_queueMutex;
    std::condition_variable m_cond2; // Not strictly needed if using one condition
};

} // namespace ai::utils
