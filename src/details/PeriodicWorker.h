#pragma once

#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace meta_log {
class PeriodicWorker
{
  public:
    PeriodicWorker(const std::function<void()>& callback_func, std::chrono::seconds interval);
    PeriodicWorker(const PeriodicWorker&) = delete;
    PeriodicWorker& operator=(const PeriodicWorker&) = delete;
    ~PeriodicWorker();

  private:
    bool active_;
    std::thread worker_thread_;
    std::mutex mtx_;
    std::condition_variable cv_;
};
}  // namespace meta_log