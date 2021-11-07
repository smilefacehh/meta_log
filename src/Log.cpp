#include "Log.h"
#include "details/Logging.h"

namespace meta_log {
void SetLogLevel(Level level) { Logging::Instance().SetLogLevel(level); }
bool IsLogLevelOn(Level level) { return Logging::Instance().IsLevelOn(level); }
void SetFlag(Flag flag) { Logging::Instance().SetFlag(flag); }
void UnSetFlag(Flag flag) { Logging::Instance().UnSetFlag(flag); }
void SetContainerMax(std::size_t n) { Logging::Instance().SetContainerMax(n); }
bool AddLogger(const std::shared_ptr<Logger>& logger) { return Logging::Instance().AddLogger(logger); }
bool RemoveLogger(const std::string& logger_name) { return Logging::Instance().RemoveLogger(logger_name); }
std::shared_ptr<Logger> Get(const std::string& logger_name) { return Logging::Instance().Get(logger_name); }
void FlushEvery(std::chrono::seconds interval) { Logging::Instance().FlushEvery(interval); }
void LogToAllLoggers(const LogMessage& log_message) { Logging::Instance().LogToAllLoggers(log_message); }
void SetPlugLoggersFunc(PlugLogFunc log_verbose, PlugLogFunc log_debug, PlugLogFunc log_info, PlugLogFunc log_warn,
                        PlugLogFunc log_error, PlugLogFunc log_fatal)
{
    Logging::Instance().SetPlugLoggersFunc(log_verbose, log_debug, log_info, log_warn, log_error, log_fatal);
}
bool IsPlugLoggerValid(Level level) { return Logging::Instance().IsPlugLoggerValid(level); }
void LogToPlugVERBOSE(const LogMessage& log_message)
{
    Logging::Instance().LogToPlugLoggers(Level::VERBOSE, log_message);
}
void LogToPlugDEBUG(const LogMessage& log_message)
{
    Logging::Instance().LogToPlugLoggers(Level::VERBOSE, log_message);
}
void LogToPlugINFO(const LogMessage& log_message) { Logging::Instance().LogToPlugLoggers(Level::INFO, log_message); }
void LogToPlugWARN(const LogMessage& log_message) { Logging::Instance().LogToPlugLoggers(Level::WARN, log_message); }
void LogToPlugERROR(const LogMessage& log_message) { Logging::Instance().LogToPlugLoggers(Level::ERROR, log_message); }
void LogToPlugFATAL(const LogMessage& log_message) { Logging::Instance().LogToPlugLoggers(Level::FATAL, log_message); }
}  // namespace meta_log