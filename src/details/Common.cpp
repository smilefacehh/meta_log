#include "Common.h"

namespace meta_log {

static const char* level_names[Level::N_LEVELS] LEVEL_NAMES;
static const char* short_level_names[Level::N_LEVELS] SHORT_LEVEL_NAMES;
static const char* color_tags[Level::N_LEVELS] COLOR_TAGS;

const char* ToNameStr(Level level)
{
    if (level >= Level::VERBOSE && level < Level::N_LEVELS)
    {
        return level_names[level];
    }
    return "UNKNOW";
}

const char* ToShortNameStr(Level level)
{
    if (level >= Level::VERBOSE && level < Level::N_LEVELS)
    {
        return short_level_names[level];
    }
    return "U";
}

Level FromNameStr(const char* name)
{
    for (int i = 0; i < Level::N_LEVELS; ++i)
    {
        if (name == level_names[i])
        {
            return static_cast<Level>(i);
        }
    }
    return Level::OFF;
}

const char* ColorTag(Level level)
{
    if (level >= Level::VERBOSE && level < Level::N_LEVELS)
    {
        return color_tags[level];
    }
    return nullptr;
}
}  // namespace meta_log