#pragma once
#include "../details/FileHelper.h"
#include "Logger.h"

namespace meta_log {
class RotateFileLogger : public Logger
{
  public:
    RotateFileLogger(const std::string& logger_name, const std::string& dir_name, const std::size_t max_size);
    ~RotateFileLogger() = default;

  private:
    void Rotate();
    void Write(const LogMessage& log_message) override;
    void Flush() override;

  private:
    const std::size_t max_size_;
    std::size_t curr_size_;
    std::string base_file_name_;
    int index_;
    FileHelper file_helper_;
};
}  // namespace meta_log