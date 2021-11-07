#pragma once

#include "../details/FileHelper.h"
#include "Logger.h"

#include <mutex>

namespace meta_log {

class BasicFileLogger : public Logger
{
  public:
    BasicFileLogger(const std::string& logger_name, const std::string& dir_name);
    ~BasicFileLogger() = default;

  private:
    void Write(const LogMessage& log_message) override;
    void Flush() override;

  private:
    std::string base_file_name_;
    FileHelper file_helper_;
};
}  // namespace meta_log