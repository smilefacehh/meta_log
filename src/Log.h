#pragma once

#include "details/Logging.h"

// 外部指定打印方法
#define PLUG_LOG_VALID(level) (meta_log::IsPlugLoggerValid(meta_log::Level::level))
#define PLUG_LOG(level)                                                                                                \
    (meta_log::LogMessage(meta_log::Level::level, __FILE__, __func__, __LINE__, &meta_log::LogToPlug##level))

// 内部打印方法
#define LOG_IS_ON(level) (meta_log::IsLogLevelOn(meta_log::Level::level))
#define LOG(level)                                                                                                     \
    (meta_log::LogMessage(meta_log::Level::level, __FILE__, __func__, __LINE__, &meta_log::LogToAllLoggers))

// 如果外部指定打印方法，则选择外部方法，否则选择内部打印方法
#define LOG_CHOOSE(level)                                                                                              \
    if (LOG_IS_ON(level)) ((PLUG_LOG_VALID(level)) ? (PLUG_LOG(level)) : (LOG(level)))

#define LOG_VERBOSE(module) LOG_CHOOSE(VERBOSE) << "<" #module ">"
#define LOG_DEBUG(module) LOG_CHOOSE(DEBUG) << "<" #module ">"
#define LOG_INFO(module) LOG_CHOOSE(INFO) << "<" #module ">"
#define LOG_WARN(module) LOG_CHOOSE(WARN) << "<" #module ">"
#define LOG_ERROR(module) LOG_CHOOSE(ERROR) << "<" #module ">"
#define LOG_FATAL(module) LOG_CHOOSE(FATAL) << "<" #module ">"

#define LOG_V(value) #value ":" << value

namespace meta_log {
void SetLogLevel(Level level);
bool IsLogLevelOn(Level level);
void SetFlag(Flag flag);
void UnSetFlag(Flag flag);
void SetContainerMax(std::size_t n);
bool AddLogger(const std::shared_ptr<Logger>& logger);
bool RemoveLogger(const std::string& logger_name);
std::shared_ptr<Logger> Get(const std::string& logger_name);
void FlushEvery(std::chrono::seconds interval);
void LogToAllLoggers(const LogMessage& log_message);
void SetPlugLoggersFunc(PlugLogFunc log_verbose, PlugLogFunc log_debug, PlugLogFunc log_info, PlugLogFunc log_warn,
                        PlugLogFunc log_error, PlugLogFunc log_fatal);
bool IsPlugLoggerValid(Level level);
void LogToPlugVERBOSE(const LogMessage& log_message);
void LogToPlugDEBUG(const LogMessage& log_message);
void LogToPlugINFO(const LogMessage& log_message);
void LogToPlugWARN(const LogMessage& log_message);
void LogToPlugERROR(const LogMessage& log_message);
void LogToPlugFATAL(const LogMessage& log_message);
}  // namespace meta_log