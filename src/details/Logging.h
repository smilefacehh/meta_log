#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "../loggers/Logger.h"
#include "Common.h"

namespace meta_log {
class Logging
{
  public:
    static Logging& Instance();

    Logging(const Logging&) = delete;
    Logging& operator=(const Logging&) = delete;

  private:
    Logging();
    ~Logging();

    Level global_level_ = Level::OFF;
    std::unordered_map<std::string, std::shared_ptr<Logger>> loggers_;

    std::mutex flush_worker_mtx_;
    std::mutex logger_map_mtx_;
};
}  // namespace meta_log