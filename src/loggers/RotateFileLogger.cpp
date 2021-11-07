#include "RotateFileLogger.h"
#include <sstream>

namespace meta_log {
RotateFileLogger::RotateFileLogger(const std::string& logger_name, const std::string& dir_name, std::size_t max_size)
    : Logger(logger_name), max_size_(max_size), curr_size_(0), index_(0)
{
    int64_t t = time(0);
    std::string file_name = FileHelper::GetFileNameWithTs(t, "log");
    base_file_name_ = FileHelper::DirectoryWithSlash(dir_name) + file_name;
    file_helper_.Open(base_file_name_, "w");
}

void RotateFileLogger::Rotate()
{
    file_helper_.Close();
    std::string new_file_name = FileHelper::GetFileNameWithIndex(base_file_name_, ++index_);
    file_helper_.Open(new_file_name, "w");
}

void RotateFileLogger::Write(const LogMessage& log_message)
{
    std::stringstream ss;
    ss << log_message.GetLogHeader(header_pattern_) << log_message.GetLogText();
    const std::string log_content(ss.str());
    std::size_t sz = log_content.size();
    curr_size_ += sz;
    if (curr_size_ > max_size_)
    {
        Rotate();
        curr_size_ = sz;
    }
    file_helper_.Write(log_content.c_str(), sz);
}

void RotateFileLogger::Flush() { file_helper_.Flush(); }

}  // namespace meta_log