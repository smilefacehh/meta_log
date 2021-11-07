#include "LogMessage.h"

#include <string.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>
#include <iomanip>
#include <sstream>

#include "Logging.h"

namespace meta_log {

LogMessage::LogMessage(Level level, const char* const file, const char* const func, int line, const LogFunc& log_func)
    : level_(level),
      file_(file),
      func_(func),
      line_(line),
      log_func_(log_func),
      flushed_(false),
      stream_buf_(),
      stream_(&stream_buf_),
      nums_to_log_(0)
{
    container_max_ = Logging::Instance().GetContainerMax();

    if (gettimeofday(&tv_, nullptr) != 0)
    {
        time(&tv_.tv_sec);
        tv_.tv_usec = 0;
    }

    Flag flag = Logging::Instance().GetFlag();
    if (flag & Flag::FIXED)
    {
        stream_ << std::fixed;
    }
}

LogMessage::~LogMessage() { Flush(); }

static inline const char* GetFileName(const char* path)
{
    const char* file = strrchr(path, '/');
    if (file) return file + 1;
    return path;
}

inline long GetThreadId()
{
    // 使用tls确保每个线程只会执行一次syscall获取线程ID
    static thread_local long thread_id = syscall(__NR_gettid);
    return thread_id;
}

std::string LogMessage::GetLogHeader(const std::string& pattern) const
{
    size_t sz = pattern.size();
    if (sz == 0)
    {
        return std::string();
    }

    thread_local struct tm result = {};
    (void)localtime_r(&tv_.tv_sec, &result);

    std::stringstream ss;
    for (size_t i = 0; i < sz; ++i)
    {
        if (pattern[i] == '%')
        {
            ++i;
            if (i < sz)
            {
                const char& c = pattern[i];
                if (c == 'Y')
                {
                    ss << std::setw(4) << 1900 + result.tm_year;
                }
                else if (c == 'M')
                {
                    ss << std::setw(2) << std::setfill('0') << 1 + result.tm_mon;
                }
                else if (c == 'D')
                {
                    ss << std::setw(2) << std::setfill('0') << result.tm_mday;
                }
                else if (c == 'h')
                {
                    ss << std::setw(2) << std::setfill('0') << result.tm_hour;
                }
                else if (c == 'm')
                {
                    ss << std::setw(2) << std::setfill('0') << result.tm_min;
                }
                else if (c == 's')
                {
                    ss << std::setw(2) << std::setfill('0') << result.tm_sec;
                }
                else if (c == 'i')
                {
                    ss << std::setw(3) << std::setfill('0') << tv_.tv_usec / 1000;
                }
                else if (c == 'V')
                {
                    ss << std::setw(5) << std::setfill(' ') << ToNameStr(level_);
                }
                else if (c == 'v')
                {
                    ss << ToShortNameStr(level_);
                }
                else if (c == 'T')
                {
                    ss << GetThreadId();
                }
                else if (c == 'F')
                {
                    ss << GetFileName(file_);
                }
                else if (c == 'L')
                {
                    ss << line_;
                }
                else if (c == 'U')
                {
                    ss << func_;
                }
            }
        }
        else
        {
            ss << pattern[i];
        }
    }

    return ss.str();
}

void LogMessage::Flush()
{
    if (flushed_) return;

    nums_to_log_ = stream_buf_.GetCount();
    if (nums_to_log_ == 0) return;

    if (stream_buf_.GetBackChar() != '\n')
    {
        // 已经保证stream_buf_至少还有两个字符的可用空间
        stream_buf_.buffer[nums_to_log_++] = '\n';
    }
    // 确保buffer是一个C风格的字符串，对于某些输出流来说会比较方便使用
    stream_buf_.buffer[nums_to_log_] = '\0';

    log_func_(*this);

    flushed_ = true;
}
}  // namespace meta_log