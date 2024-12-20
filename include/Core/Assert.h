#ifndef _ASSERT_H
#define _ASSERT_H

#include "Core/Log.hpp"

// This header defines the ASSERT and CORE ASSERT macros.
#ifndef ISABLE_ASSERT
    #define ASSERT(condition, message) \
    { \
        if (!(condition)) { \
            Log::error("Assertion failed: (") << #condition << ") " << message \
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
#endif