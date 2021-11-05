#pragma once

#include <chrono>
#include <functional>

#include "details/Common.h"

namespace meta_log {
void SetLogLevel();
bool CreateLogger(const char* name, );
bool AddLogger();
void RemoveLogger();
void FlushEvery(std::chrono::seconds interval);
void SetFlag(Flag flag);
void UnSetFlag(Flag flag);
void SetContainerMax(int n);
}  // namespace meta_log