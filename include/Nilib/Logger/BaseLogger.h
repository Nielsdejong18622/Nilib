#ifndef _BASE_LOGGER_H
#define _BASE_LOGGER_H

namespace Nilib
{

    // Empty dummy Logger class for when logging is not defined for that level.
    // This class should be optimized away by the compiler.
    struct BaseLogger
    {
        template <typename T>
        constexpr BaseLogger &operator<<(T const &message)
        {
            return *this;
        }
    };

};
#endif