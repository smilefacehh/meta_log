#include "PeriodicWorker.h"

namespace meta_log {
PeriodicWorker::PeriodicWorker(const std::function<void()>& callback_func, std::chrono::seconds interval)
{
    active_ = (interval > std::chrono::seconds::zero());

    if (!active_) return;

    worker_thread_ = std::thread([this, callback_func, interval]() {
        for (;;)
        {
            std::unique_lock<std::mutex> lock(mtx_);
            if (cv_.wait_for(lock, interval, [this] { return !active_; }))
            {
                return;
            }
            callback_func();
        }
    });
}

PeriodicWorker::~PeriodicWorker()
{
    if (worker_thread_.joinable())
    {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            active_ = false;
        }
        cv_.notify_one();
        worker_thread_.join();
    }
}
}  // namespace meta_log