#ifndef _ASSERT_H
#define _ASSERT_H

// This header defines the ASSERT and CORE ASSERT macros.
#include "Nilib/Logger/Log.hpp"

namespace Nilib{

#ifndef ISABLE_ASSERT
    #define ASSERT(condition, message) \
    { \
        if (!(condition)) { \
            LOG_ERROR() << "Assertion failed: (" << #condition << ") " << message \
                      << " in function " << __func__ \
                      << " in file " << __FILE__ \
                      << " at line " << __LINE__ << '\n'; \
            std::exit(EXIT_FAILURE); \
        } \
    } \

    #define CORE_ASSERT(condition) ASSERT(condition, "")

#else
	#define ASSERT(...)

    #define CORE_ASSERT(...)
#endif

}
#endif