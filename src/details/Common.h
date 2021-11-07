#pragma once
#include <memory>

namespace meta_log {

// 日志级别
enum Level
{
    VERBOSE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5,
    OFF = 6,
    N_LEVELS
};

// 一些控制标记
enum Flag : uint64_t
{
    NONE = 0,
    FIXED = 1L << 0
};

#ifndef LEVEL_NAMES
#define LEVEL_NAMES                                                                                                    \
    {                                                                                                                  \
        "VERBOSE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "OFF"                                                    \
    }
#endif

#ifndef SHORT_LEVEL_NAMES
#define SHORT_LEVEL_NAMES                                                                                              \
    {                                                                                                                  \
        "V", "D", "I", "W", "E", "F", "O"                                                                              \
    }
#endif

#ifndef COLOR_TAGS
#define COLOR_TAGS                                                                                                     \
    {                                                                                                                  \
        "\033[35m", "\033[36m", "\033[32m", "\033[33m", "\033[31m", "\033[31m", "\033[0m"                              \
    }
#endif

#define COLOR_END_TAG "\033[0m"

// Level枚举转名字
const char* ToNameStr(Level level);

// Level枚举转名字缩写
const char* ToShortNameStr(Level level);

// 名字转Level枚举
Level FromNameStr(const char* name);

// Level枚举获取对应颜色标签
const char* ColorTag(Level level);

// 日志头部格式串
// %Y 年
// %M 月
// %D 日
// %h 小时
// %m 分钟
// %s 秒
// %i 毫秒
// %V 日志等级全称
// %v 日志等级缩写
// %T 线程号
// %F 文件名
// %L 行号
// %U 调用函数名
#define HEADER_PATTERN_SET "YMDhmsiVvTFLU"

// 日志头部默认格式，[年-月-日 时:分:秒.毫秒][级别][线程][文件:行][函数]
#define DEFAULT_HEADER_PATTERN "[%Y-%M-%D %h:%m:%s.%i][%V][%T][%F:%L][%U]"

// C++14
#if __cplusplus >= 201402L
using std::make_unique;
#else
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    static_assert(!std::is_array<T>::value, "arrays not supported");
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif
}  // namespace meta_log