#ifndef _ENUM_HPP
#define _ENUM_HPP

#include <iostream>

namespace Nilib
{
#define ENUM(EnumName, ...)                                                          \
    class EnumName                                                                   \
    {                                                                                \
    public:                                                                          \
        enum value : int                                                             \
        {                                                                            \
            __VA_ARGS__                                                              \
        };                                                                           \
        EnumName()                                                                   \
        {                                                                            \
        }                                                                            \
        EnumName(value v) : val(v)                                                   \
        {                                                                            \
        }                                                                            \
                                                                                     \
        operator std::string() const { return toString(); }                          \
        operator int() const { return static_cast<int>(val); }                       \
                                                                                     \
        std::string toString() const                                                 \
        {                                                                            \
            auto it = valueToStringMap().find(val);                                  \
            return it != valueToStringMap().end() ? it->second : "UNKNOWN";          \
        }                                                                            \
                                                                                     \
        [[nodiscard]] static EnumName fromString(std::string const &str)             \
        {                                                                            \
            for (auto const &[num, name] : valueToStringMap())                       \
            {                                                                        \
                bool contains_int = false;                                           \
                int idx;                                                             \
                size_t pos;                                                          \
                try                                                                  \
                {                                                                    \
                    idx = std::stoi(str, &pos);                                      \
                    contains_int = pos == str.size();                                \
                }                                                                    \
                catch (...)                                                          \
                {                                                                    \
                }                                                                    \
                if (contains_int)                                                    \
                    return EnumName(static_cast<value>(idx));                        \
                if (name == str)                                                     \
                    return EnumName(num);                                            \
            }                                                                        \
            throw std::invalid_argument("Invalid enum string: " + str);              \
        }                                                                            \
                                                                                     \
        [[nodiscard]] static EnumName fromName(std::string const &str)               \
        {                                                                            \
            for (auto const &[num, name] : valueToStringMap())                       \
            {                                                                        \
                if (name == str)                                                     \
                    return EnumName(num);                                            \
            }                                                                        \
            throw std::invalid_argument("Invalid enum name: " + str);                \
        }                                                                            \
                                                                                     \
        friend std::stringstream &operator>>(std::stringstream &os, EnumName &self)  \
        {                                                                            \
            std::string token;                                                       \
            os >> token;                                                             \
            self = fromString(token);                                                \
            return os;                                                               \
        }                                                                            \
                                                                                     \
        [[nodiscard]] static EnumName fromInt(int const i)                           \
        {                                                                            \
            for (auto const &[num, name] : valueToStringMap())                       \
            {                                                                        \
                if (num == i)                                                        \
                    return EnumName(num);                                            \
            }                                                                        \
            throw std::invalid_argument("Invalid enum value: " + i);                 \
        }                                                                            \
                                                                                     \
        bool operator==(EnumName const &other) const { return val == other.val; }    \
        bool operator==(EnumName::value const &other) const { return val == other; } \
        bool operator!=(EnumName const &other) const { return !(*this == other); }   \
                                                                                     \
        void print() const                                                           \
        {                                                                            \
            LOG_PROGRESS("Printing", #EnumName, "enum");                             \
            for (auto const &[num, name] : valueToStringMap())                       \
                LOG_PROGRESS(name, '=', num);                                        \
        };                                                                           \
        constexpr static char const *description() { return #__VA_ARGS__; };         \
                                                                                     \
    private:                                                                         \
        value val;                                                                   \
        static const std::unordered_map<value, std::string> &valueToStringMap()      \
        {                                                                            \
            static std::unordered_map<value, std::string> const map = [] {           \
                std::unordered_map<value, std::string> m;                            \
                std::string const names = #__VA_ARGS__;                              \
                size_t pos = 0;                                                      \
                int enumVal = 0;                                                     \
                while (pos < names.size()) {                                         \
                    size_t comma = names.find(',', pos);                             \
                    std::string token = names.substr(pos, comma - pos);              \
                    size_t eq = token.find('=');                                     \
                    std::string name = token.substr(0, eq);                          \
                    name.erase(0, name.find_first_not_of(" \t\n"));                  \
                    name.erase(name.find_last_not_of(" \t\n") + 1);                  \
                    if (eq != std::string::npos) {                                   \
                        enumVal = std::stoi(token.substr(eq + 1));                   \
                    }                                                                \
                    while(m.contains(static_cast<value>(enumVal))) ++enumVal;        \
                    m[static_cast<value>(enumVal)] = name;                           \
                    ++enumVal;                                                       \
                    if (comma == std::string::npos) break;                           \
                    pos = comma + 1;                                                 \
                }                                                                    \
                return m; }();      \
            return map;                                                              \
        }                                                                            \
    }

} // namespace Nilib

#endif