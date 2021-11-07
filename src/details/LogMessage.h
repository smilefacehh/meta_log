#pragma once

#include <functional>
#include <ostream>

#include "Common.h"
#include "IterableContainer.h"

#define BUFFER_SIZE 4096

namespace meta_log {

class LogStreamBuf : public std::streambuf
{
  public:
    friend class LogMessage;

    LogStreamBuf()
    {
        static_assert(BUFFER_SIZE > 0, "kBufferSize must be large than 2");
        setp(buffer, buffer + BUFFER_SIZE);

        buffer[0] = '\0';
    }

    ~LogStreamBuf() override = default;

    LogStreamBuf(const LogStreamBuf&) = delete;
    LogStreamBuf& operator=(const LogStreamBuf&) = delete;

    /**
     * 获取缓存区
     * @return 缓冲区起始地址
     */
    const char* GetBuffer() const { return buffer; }

    /**
     * 获取buffer的已用空间大小
     * @return buffer的已用空间大小
     */
    size_t GetCount() const { return pptr() - pbase(); }

    /**
     * 获取缓冲区最后一个字符
     * @return 如果缓冲区非空，返回最后一个字符;否则返回EOF
     */
    char GetBackChar() const
    {
        if (GetCount() == 0) return EOF;
        return *(pptr() - 1);
    }

  protected:
    // 实现std::streambuf的overflow接口，当数据写入并且缓冲区已满时会被调用
    int_type overflow(int_type ch) override { return ch; }

  private:
    // 固定大小缓冲区（预留2个字符）
    char buffer[BUFFER_SIZE + 2 + 1];
};

class LogMessage
{
  public:
    typedef std::function<void(const LogMessage&)> LogFunc;

    LogMessage(Level level, const char* file, const char* func, int line, const LogFunc& log_func);
    ~LogMessage();

// 基础数据类型
#define BASIC_SIMPLE_LOG(BASIC_TYPE)                                                                                   \
    inline LogMessage& operator<<(BASIC_TYPE msg)                                                                      \
    {                                                                                                                  \
        stream_ << msg;                                                                                                \
        return *this;                                                                                                  \
    }

    BASIC_SIMPLE_LOG(char)
    BASIC_SIMPLE_LOG(bool)
    BASIC_SIMPLE_LOG(signed short)
    BASIC_SIMPLE_LOG(unsigned short)
    BASIC_SIMPLE_LOG(signed int)
    BASIC_SIMPLE_LOG(unsigned int)
    BASIC_SIMPLE_LOG(signed long)
    BASIC_SIMPLE_LOG(unsigned long)
    BASIC_SIMPLE_LOG(float)
    BASIC_SIMPLE_LOG(double)
    BASIC_SIMPLE_LOG(char*)
    BASIC_SIMPLE_LOG(const char*)
    BASIC_SIMPLE_LOG(const void*)
    BASIC_SIMPLE_LOG(long double)

    inline LogMessage& operator<<(const std::string& msg)
    {
        stream_ << msg;
        return *this;
    }
    inline LogMessage& operator<<(std::ostream& (*ostream_fp)(std::ostream&))
    {
        stream_ << ostream_fp;
        return *this;
    }
    inline LogMessage& operator<<(const std::ios_base& (*ios_base_fp)(std::ios_base&))
    {
        stream_ << ios_base_fp;
        return *this;
    }

    template <typename Iterator>
    inline LogMessage& WriteIterator(Iterator it_begin, Iterator it_end, std::size_t sz)
    {
        stream_ << "[";
        for (std::size_t i = 0; it_begin != it_end && i < container_max_; ++i, ++it_begin)
        {
            operator<<(*it_begin);
            stream_ << ((i < sz - 1) ? "," : "");
        }
        if (it_begin != it_end)
        {
            stream_ << "...";
        }
        stream_ << "]";

        return *this;
    }

// STL容器类型
#define ITERATOR_CONTAINER_LOG_ONE_ARG(CONTAINER_TYPE)                                                                 \
    template <typename T>                                                                                              \
    inline LogMessage& operator<<(const CONTAINER_TYPE<T>& container)                                                  \
    {                                                                                                                  \
        return WriteIterator(container.begin(), container.end(), container.size());                                    \
    }
#define ITERATOR_CONTAINER_LOG_TWO_ARG(CONTAINER_TYPE)                                                                 \
    template <typename T1, typename T2>                                                                                \
    inline LogMessage& operator<<(const CONTAINER_TYPE<T1, T2>& container)                                             \
    {                                                                                                                  \
        return WriteIterator(container.begin(), container.end(), container.size());                                    \
    }
#define ITERATOR_CONTAINER_LOG_THREE_ARG(CONTAINER_TYPE)                                                               \
    template <typename T1, typename T2, typename T3>                                                                   \
    inline LogMessage& operator<<(const CONTAINER_TYPE<T1, T2, T3>& container)                                         \
    {                                                                                                                  \
        return WriteIterator(container.begin(), container.end(), container.size());                                    \
    }
#define ITERATOR_CONTAINER_LOG_FOUR_ARG(CONTAINER_TYPE)                                                                \
    template <typename T1, typename T2, typename T3, typename T4>                                                      \
    inline LogMessage& operator<<(const CONTAINER_TYPE<T1, T2, T3, T4>& container)                                     \
    {                                                                                                                  \
        return WriteIterator(container.begin(), container.end(), container.size());                                    \
    }
#define ITERATOR_CONTAINER_LOG_FIVE_ARG(CONTAINER_TYPE)                                                                \
    template <typename T1, typename T2, typename T3, typename T4, typename T5>                                         \
    inline LogMessage& operator<<(const CONTAINER_TYPE<T1, T2, T3, T4, T5>& container)                                 \
    {                                                                                                                  \
        return WriteIterator(container.begin(), container.end(), container.size());                                    \
    }

    ITERATOR_CONTAINER_LOG_TWO_ARG(std::vector)
    ITERATOR_CONTAINER_LOG_TWO_ARG(std::list)
    ITERATOR_CONTAINER_LOG_TWO_ARG(std::deque)
    ITERATOR_CONTAINER_LOG_THREE_ARG(std::set)
    ITERATOR_CONTAINER_LOG_THREE_ARG(std::multiset)
    ITERATOR_CONTAINER_LOG_FOUR_ARG(std::map)
    ITERATOR_CONTAINER_LOG_FOUR_ARG(std::multimap)

    // 迭代器适配容器
    template <typename T, typename Container>
    inline LogMessage& operator<<(const std::queue<T, Container>& q)
    {
        //        IterableQueue<T, Container> iterable_queue = static_cast<IterableQueue<T, Container>>(q);
        IterableQueue<T, Container> iterable_queue(q, container_max_);
        return WriteIterator(iterable_queue.begin(), iterable_queue.end(), iterable_queue.size());
    }

    template <typename T, typename Container>
    inline LogMessage& operator<<(const std::stack<T, Container>& s)
    {
        //        IterableStack<T, Container> iterable_stack = static_cast<IterableStack<T, Container>>(s);
        IterableStack<T, Container> iterable_stack(s, container_max_);
        return WriteIterator(iterable_stack.begin(), iterable_stack.end(), iterable_stack.size());
    }

    template <typename T, typename Container, typename Compare>
    inline LogMessage& operator<<(const std::priority_queue<T, Container, Compare>& q)
    {
        //        IterablePriorityQueue<T, Container, Compare> iterable_priority_queue =
        //            static_cast<IterablePriorityQueue<T, Container, Compare>>(q);
        IterablePriorityQueue<T, Container, Compare> iterable_priority_queue(q, container_max_);
        return WriteIterator(iterable_priority_queue.begin(), iterable_priority_queue.end(),
                             iterable_priority_queue.size());
    }

    template <typename First, typename Second>
    inline LogMessage& operator<<(const std::pair<First, Second>& p)
    {
        stream_ << "(";
        operator<<(static_cast<First>(p.first));
        stream_ << ",";
        operator<<(static_cast<Second>(p.second));
        stream_ << ")";

        return *this;
    }

    size_t GetLogTextLength() const { return nums_to_log_; }

    const char* GetLogText() const { return stream_buf_.GetBuffer(); }

    Level GetLevel() const { return level_; }

    std::string GetLogHeader(const std::string& header_pattern) const;

  private:
    void Flush();

  private:
    // 日志等级
    const Level level_;
    // 打印日志的文件名
    const char* const file_;
    // 打印日志的函数名
    const char* const func_;
    // 打印日志的文件行数
    const int line_;
    // 打印日志的时间
    struct timeval tv_;
    // 日志输出后端
    LogFunc log_func_;
    // 记录日志是否已经输出
    bool flushed_;
    // 日志流的缓冲区
    LogStreamBuf stream_buf_;
    // 日志流
    std::ostream stream_;
    // 日志文本的字符数
    std::size_t nums_to_log_;
    // 容器类型最多打印元素数量
    std::size_t container_max_;
};

}  // namespace meta_log