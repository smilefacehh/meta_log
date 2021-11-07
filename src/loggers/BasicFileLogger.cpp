#include "BasicFileLogger.h"
#include <sstream>

namespace meta_log {
BasicFileLogger::BasicFileLogger(const std::string& logger_name, const std::string& dir_name) : Logger(logger_name)
{
    int64_t t = time(0);
    std::string file_name = FileHelper::GetFileNameWithTs(t, "log");
    base_file_name_ = FileHelper::DirectoryWithSlash(dir_name) + file_name;
    file_helper_.Open(base_file_name_, "w");
}

void BasicFileLogger::Write(const LogMessage& log_message)
{
    std::stringstream ss;
    ss << log_message.GetLogHeader(header_pattern_) << log_message.GetLogText();
    const std::string log_content(ss.str());
    std::size_t sz = log_content.size();
    file_helper_.Write(log_content.c_str(), sz);
}

void BasicFileLogger::Flush() { file_helper_.Flush(); }
}  // namespace meta_log