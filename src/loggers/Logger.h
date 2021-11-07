#pragma once

#include <string>

#include "../details/Common.h"
#include "../details/LogMessage.h"

namespace meta_log {

class Logger
{
  public:
    explicit Logger(const std::string& name)
        : name_(name), header_pattern_(DEFAULT_HEADER_PATTERN), header_pattern_set_(HEADER_PATTERN_SET)
    {
    }

    virtual ~Logger() = default;

    virtual void Write(const LogMessage& log_message) = 0;

    virtual void Flush() = 0;

    const std::string& GetName() const { return name_; }

    bool SetHeaderPattern(const std::string& header_pattern)
    {
        if (IsHeaderPatternValid(header_pattern))
        {
            header_pattern_ = header_pattern;
            return true;
        }
        return false;
    }

    const std::string& GetHeaderPattern() const { return header_pattern_; }

  protected:
    inline bool IsHeaderPatternValid(const std::string& header_pattern)
    {
        size_t sz = header_pattern.size();
        if (sz == 0)
        {
            return false;
        }
        for (size_t i = 0; i < sz; ++i)
        {
            if (header_pattern[i] == '%')
            {
                ++i;
                if (i > sz || header_pattern_set_.find(header_pattern[i]) == std::string::npos)
                {
                    return false;
                }
            }
        }
        return true;
    }

  protected:
    // Logger名字
    const std::string name_;

    // Logger头部格式
    std::string header_pattern_;

    const std::string header_pattern_set_;
};
}  // namespace meta_log