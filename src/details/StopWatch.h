#pragma once

#include <chrono>
#include <ostream>
#include "LogMessage.h"

namespace meta_log {

class StopWatch
{
    using clock = std::chrono::steady_clock;
    std::chrono::time_point<clock> start_tp_;

  public:
    StopWatch() : start_tp_(clock::now()) {}

    std::chrono::duration<double> elapsed() const { return std::chrono::duration<double>(clock::now() - start_tp_); }

    void reset() { start_tp_ = clock ::now(); }

    friend LogMessage& operator<<(LogMessage& lm, const StopWatch& sw)
    {
        lm << sw.elapsed().count();
        return lm;
    }
};
}  // namespace meta_log