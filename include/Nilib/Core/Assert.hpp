#ifndef _ASSERT_H
#define _ASSERT_H

// This header defines the ASSERT and CORE ASSERT macros.
#include "Nilib/Logger/Log.hpp"
#include <assert.h>

namespace Nilib
{

#ifndef ASSERT_OFF

#define ASSERT(condition, ...)                             \
    do                                                     \
    {                                                      \
        if (!(condition))                                  \
        {                                                  \
            LOG_ERROR("Assertion failed: {}", #condition); \
            __VA_OPT__(LOG_ERROR(__VA_ARGS__);)            \
        }                                                  \
    } while (false)

#define CORE_ASSERT(condition)                                                \
    do                                                                        \
    {                                                                         \
        if (!(condition))                                                     \
        {                                                                     \
            LOG_ERROR("Assertion ({}) failed!", #condition);                  \
            LOG_ERROR("Call: __{}__ in {}:{}", __func__, __FILE__, __LINE__); \
            std::abort();                                                     \
        }                                                                     \
    } while (false)

#else
#define ASSERT(...) \
    {               \
    }

#define CORE_ASSERT(...) \
    {                    \
    }
#endif

}
#endif