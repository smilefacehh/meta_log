#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "../loggers/Logger.h"
#include "Common.h"
#include "LogMessage.h"
#include "PeriodicWorker.h"

namespace meta_log {

typedef std::function<void(const char*)> PlugLogFunc;

class Logging
{
  public:
    static Logging& Instance();

    Logging(const Logging&) = delete;
    Logging& operator=(const Logging&) = delete;

    // todo 每条消息都要获取flag和下面的配置，要加锁，如果效率不行就用宏定义，用户在包含头文件之前定义宏
    void SetFlag(Flag flag);
    void UnSetFlag(Flag flag);
    Flag GetFlag();
    void SetContainerMax(int n);
    std::size_t GetContainerMax();
    // end
    void SetLogLevel(Level level);
    Level GetLogLevel();
    bool IsLevelOn(Level level);
    bool AddLogger(const std::shared_ptr<Logger>& logger);
    bool RemoveLogger(const std::string& logger_name);
    std::shared_ptr<Logger> Get(const std::string& logger_name);
    void FlushEvery(std::chrono::seconds interval);
    void FlushAll();
    void LogToAllLoggers(const LogMessage& log_message);

    void SetPlugLoggersFunc(PlugLogFunc log_verbose, PlugLogFunc log_debug, PlugLogFunc log_info, PlugLogFunc log_warn,
                            PlugLogFunc log_error, PlugLogFunc log_fatal);
    bool IsPlugLoggerValid(Level level);
    void LogToPlugLoggers(Level level, const LogMessage& log_message);

  private:
    Logging();
    ~Logging();

    Flag flag_ = Flag::NONE;
    std::size_t container_max_ = 100;
    Level global_level_ = Level::OFF;
    std::unordered_map<std::string, std::shared_ptr<Logger>> loggers_;
    std::unique_ptr<PeriodicWorker> flush_worker_;

    std::mutex flag_mtx_;
    std::mutex container_max_mtx_;
    std::mutex log_level_mtx_;
    std::mutex logger_map_mtx_;
    std::mutex flush_worker_mtx_;

    PlugLogFunc plug_log_func_[Level::N_LEVELS] = {nullptr};
};
}  // namespace meta_log