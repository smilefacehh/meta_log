#include "ConsoleLogger.h"

#include <iostream>

namespace meta_log {
ConsoleLogger::ConsoleLogger(const std::string& name) : Logger(name) {}

void ConsoleLogger::Write(const LogMessage& log_message)
{
    std::lock_guard<std::mutex> lock_guard(write_mtx_);

    const char* color_tag = ColorTag(log_message.GetLevel());
    if (color_tag != nullptr)
    {
        std::cout << color_tag << log_message.GetLogHeader(header_pattern_) << COLOR_END_TAG
                  << log_message.GetLogText();
    }
    else
    {
        std::cout << log_message.GetLogHeader(header_pattern_) << log_message.GetLogText();
    }
    std::cout.flush();
}

void ConsoleLogger::Flush()
{
    std::lock_guard<std::mutex> lock_guard(write_mtx_);
    std::cout.flush();
}
}  // namespace meta_log