#pragma once

#include "Logger.h"

#include <mutex>

namespace meta_log {

class ConsoleLogger : public Logger
{
  public:
    ConsoleLogger(const std::string& name);

    ~ConsoleLogger() = default;

    void Write(const LogMessage& log_message) override;
    void Flush() override;

  private:
    std::mutex write_mtx_;
};
}  // namespace meta_log