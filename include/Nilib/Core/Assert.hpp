#ifndef _ASSERT_H
#define _ASSERT_H

// This header defines the ASSERT and CORE ASSERT macros.
#include "Nilib/Logger/Log.hpp"

namespace Nilib
{

#ifndef ISABLE_ASSERT

#define ASSERT(condition, ...)                                                                  \
    {                                                                                           \
        if (!(condition))                                                                       \
        {                                                                                       \
            LOG_ERROR("Assertion (", #condition, ") failed!", __VA_OPT__(__VA_ARGS__));         \
            LOG_ERROR() << "Call: __" << __func__ << "__ in " << __FILE__ << ':' << __LINE__ << '\n'; \
            ::Nilib::LoggerPool::instance().flush_loggers();                                    \
            std::exit(EXIT_FAILURE);                                                            \
        }                                                                                       \
    }

#define CORE_ASSERT(condition) ASSERT(condition, "")

#else
#define ASSERT(...)

#define CORE_ASSERT(...)
#endif

}
#endif