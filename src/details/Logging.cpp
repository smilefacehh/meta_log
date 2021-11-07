#include "Logging.h"

namespace meta_log {
Logging& Logging::Instance()
{
    static Logging logging;
    return logging;
}

Logging::Logging() {}

Logging::~Logging() { flush_worker_.reset(); }

void Logging::SetFlag(Flag flag)
{
    std::lock_guard<std::mutex> lg(flag_mtx_);
    flag_ = static_cast<Flag>(static_cast<uint64_t>(flag_) | static_cast<uint64_t>(flag));
}

void Logging::UnSetFlag(Flag flag)
{
    std::lock_guard<std::mutex> lg(flag_mtx_);
    if (flag_ & flag)
    {
        flag_ = static_cast<Flag>(static_cast<uint64_t>(flag_) - static_cast<uint64_t>(flag));
    }
}

Flag Logging::GetFlag()
{
    std::lock_guard<std::mutex> lg(flag_mtx_);
    return flag_;
}

void Logging::SetContainerMax(int n)
{
    std::lock_guard<std::mutex> lg(container_max_mtx_);
    container_max_ = n;
}

std::size_t Logging::GetContainerMax()
{
    std::lock_guard<std::mutex> lg(container_max_mtx_);
    return container_max_;
}

void Logging::SetLogLevel(Level level)
{
    std::lock_guard<std::mutex> lg(log_level_mtx_);
    global_level_ = level;
}

Level Logging::GetLogLevel()
{
    std::lock_guard<std::mutex> lg(log_level_mtx_);
    return global_level_;
}

bool Logging::IsLevelOn(Level level)
{
    std::lock_guard<std::mutex> lg(log_level_mtx_);
    return level >= global_level_;
}

bool Logging::AddLogger(const std::shared_ptr<Logger>& logger)
{
    std::lock_guard<std::mutex> lg(logger_map_mtx_);
    if (logger == nullptr) return false;
    if (loggers_.find(logger->GetName()) != loggers_.end()) return false;
    loggers_.insert(std::pair<std::string, std::shared_ptr<Logger>>(logger->GetName(), logger));
    return true;
}

bool Logging::RemoveLogger(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lg(logger_map_mtx_);
    if (loggers_.find(logger_name) == loggers_.end()) return false;
    loggers_.erase(logger_name);
    return true;
}

std::shared_ptr<Logger> Logging::Get(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lg(logger_map_mtx_);
    if (loggers_.find(logger_name) == loggers_.end()) return nullptr;
    return loggers_[logger_name];
}

void Logging::FlushEvery(std::chrono::seconds interval)
{
    auto callback = [this]() { this->FlushAll(); };
    flush_worker_ = make_unique<PeriodicWorker>(callback, interval);
}

void Logging::FlushAll()
{
    for (auto& elem : loggers_)
    {
        elem.second->Flush();
    }
}

void Logging::LogToAllLoggers(const LogMessage& log_message)
{
    for (auto& elem : loggers_)
    {
        elem.second->Write(log_message);
    }
}

void Logging::SetPlugLoggersFunc(PlugLogFunc log_verbose, PlugLogFunc log_debug, PlugLogFunc log_info,
                                 PlugLogFunc log_warn, PlugLogFunc log_error, PlugLogFunc log_fatal)
{
    plug_log_func_[Level::VERBOSE] = log_verbose;
    plug_log_func_[Level::DEBUG] = log_debug;
    plug_log_func_[Level::INFO] = log_info;
    plug_log_func_[Level::WARN] = log_warn;
    plug_log_func_[Level::ERROR] = log_error;
    plug_log_func_[Level::FATAL] = log_fatal;
}

bool Logging::IsPlugLoggerValid(Level level)
{
    if (level >= Level::VERBOSE && level < Level::N_LEVELS)
    {
        return plug_log_func_[level] != nullptr;
    }
    return false;
}

void Logging::LogToPlugLoggers(Level level, const LogMessage& log_message)
{
    if (level >= Level::VERBOSE && level < Level::N_LEVELS)
    {
        if (plug_log_func_[level] != nullptr)
        {
            plug_log_func_[level](log_message.GetLogText());
        }
    }
}
}  // namespace meta_log
