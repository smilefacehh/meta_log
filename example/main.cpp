#include <unistd.h>

#include "../src/Log.h"
#include "../src/details/StopWatch.h"
#include "../src/loggers/BasicFileLogger.h"
#include "../src/loggers/ConsoleLogger.h"
#include "../src/loggers/RotateFileLogger.h"

int main()
{
    std::shared_ptr<meta_log::ConsoleLogger> console_logger =
        std::make_shared<meta_log::ConsoleLogger>("console_logger");
    console_logger->SetHeaderPattern("[%Y-%M-%D %h:%m:%s.%i][%T][%v][%F:%L][%U]");
    meta_log::AddLogger(console_logger);

    std::shared_ptr<meta_log::RotateFileLogger> rotate_file_logger =
        std::make_shared<meta_log::RotateFileLogger>("basic_file_logger", "./log", 1024 * 1024 * 10);
    meta_log::AddLogger(rotate_file_logger);

    meta_log::SetLogLevel(meta_log::Level::VERBOSE);
    meta_log::SetFlag(meta_log::Flag::FIXED);
    meta_log::SetContainerMax(2);
    meta_log::FlushEvery(std::chrono::seconds(2));

    // 测试

    {
        LOG_VERBOSE(verbose) << "verbose log";
        LOG_DEBUG(debug) << "debug log";
        LOG_INFO(info) << "info log";
        LOG_WARN(warn) << "warn log";
        LOG_ERROR(error) << "error log";
        LOG_FATAL(fatal) << "fatal log";
    }
    {
        LOG_VERBOSE(--) << "基础数据类型";
        int a = 1024;
        bool b = false;
        float c = 12.123456789;
        double d = 123456789.123456789;
        char e[20] = "test str";

        LOG_DEBUG(int) << a;
        LOG_DEBUG(bool) << b;
        LOG_DEBUG(float) << c;
        LOG_DEBUG(double) << d;
        LOG_DEBUG(char*) << e;
    }

    {
        LOG_VERBOSE(--) << "容器类型:";
        std::vector<std::string> a{"str1", "str2"};
        std::vector<std::vector<int>> b{{1, 2}, {3, 4, 5}, {4, 5}, {6, 7}};
        std::list<int> c{1, 2, 3};
        std::deque<int> d{1, 2, 3};
        std::set<int> e{1, 2, 3};

        std::map<int, std::string> f;
        f.insert(std::pair<int, std::string>(1, "str1"));
        f.insert(std::pair<int, std::string>(2, "str2"));

        std::multiset<int> g{1, 1, 2, 3};

        std::multimap<int, std::string> h;
        h.insert(std::pair<int, std::string>(1, "str1"));
        h.insert(std::pair<int, std::string>(1, "str2"));
        h.insert(std::pair<int, std::string>(2, "str3"));

        std::queue<int> i;
        i.push(1);
        i.push(2);
        i.push(3);
        i.push(4);

        std::stack<int> j;
        j.push(1);
        j.push(2);
        j.push(3);
        j.push(4);

        LOG_DEBUG(vector) << a;
        LOG_DEBUG(vector<vector>) << b;
        LOG_DEBUG(list) << c;
        LOG_DEBUG(deque) << d;
        LOG_DEBUG(set) << e;
        LOG_DEBUG(map) << f;
        LOG_DEBUG(multiset) << g;
        LOG_DEBUG(multimap) << h;
        LOG_DEBUG(queue) << i;
        LOG_DEBUG(stack) << j;
    }
    {
        LOG_VERBOSE(--) << "fixed格式:";
        LOG_DEBUG(float) << 123.456789;
        LOG_DEBUG(int) << 1e6;
        meta_log::UnSetFlag(meta_log::Flag::FIXED);
        LOG_DEBUG(float) << 123.456789;
        LOG_DEBUG(int) << 1e6;
        meta_log::SetFlag(meta_log::Flag::FIXED);
    }
    {
        LOG_VERBOSE(--) << "stopwatch用法:";
        meta_log::StopWatch sw;
        LOG_DEBUG(stopwatch) << "start to sleep 1s";
        usleep(1 * 1e6);
        LOG_DEBUG(stopwatch) << "cost:" << sw << "s";
        sw.reset();
        LOG_DEBUG(stopwatch) << "start to sleep 0.5s";
        usleep(0.5 * 1e6);
        LOG_DEBUG(stopwatch) << "cost:" << sw << "s";
    }

    return 0;
}