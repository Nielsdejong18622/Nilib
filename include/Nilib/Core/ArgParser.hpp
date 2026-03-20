#ifndef ARGS_H
#define ARGS_H

#include "Nilib/Core/Assert.hpp"
#include "Nilib/Core/Enum.hpp"
#include "Nilib/Logger/Log.hpp"
#include <functional>
#include <algorithm>

namespace Nilib
{

    class ArgParser
    {
    public:
        // Create an ArgParser object. Call parse or parse_or_exit to obtain the
        ArgParser(std::string_view program_description, std::string_view version);
        ~ArgParser() = default;
        ArgParser(ArgParser const &) = delete;
        ArgParser(ArgParser const &&) = delete;

        // Sets store = true; iff flag_char, -flag_char or --flag_char flag_name, -flag_name or --flag_name is found in argv (otherwise sets store = false).
        void flag(bool &store, std::string_view flag_name, std::string_view description, char flag_char);

        // An option such as: --option=value, --option value, -option value -option
        // Attempts to convert option to value and store it in store.
        template <typename type>
        void option(type &store, std::string_view option_name, std::string_view description, bool required = false);

        // Handle as positional argument. For example, programname file1.txt
        // Converts argument to type and saves in store. If conversion fails,
        template <typename type>
        void argument(type &store, std::string_view description);

        // Obtain the program name.
        std::string_view program_name() const;

        // Obtain number of arguments (argc).
        int argc() const;

        // Parses commandline arguments.
        // Returns true if succes, otherwise false.
        [[nodiscard]] bool parse(int argc, char **argv);

        // Parses commandline arguments. Or exits with help printed.
        void parse_or_exit(int argc, char **argv);

        // Prints help text.
        void help() const;

    private:
        template <typename type>
        static type convert(std::string_view value);
        bool all_parsed() const;

        struct CLArg
        {
            enum class Type : int
            {
                ARGUMENT = 0,
                OPTION = 1,
                FLAG = 2
            } type; // Sort on type.

            std::string short_name;
            std::string long_name;
            std::string description;
            uint16_t pos; // Position we expect this CLArg at (if pARG)
            bool required;
            bool parsed;
            std::function<void(std::string_view)> set_val;

            bool operator<(CLArg const &other) const { return static_cast<int>(type) < static_cast<int>(other.type); }
        };

        // Stack of CLARG as set by the user.
        std::vector<CLArg> d_clargs;
        std::vector<std::string> d_unrecognized;
        std::string d_program_name;
        std::string d_program_description;
        std::string d_version;
        bool d_display_help;
        int d_argc;
    };

    // Helper conversion function.
    template <typename type>
    type ArgParser::convert(std::string_view value)
    {
        try
        {
            if constexpr (std::is_same_v<type, bool>)
            {
                if (value == "true" || value == "1")
                    return true;
                if (value == "false" || value == "0")
                    return false;
                ASSERT(false, "Unsupported boolean type!");
                return false;
            }
            else if constexpr (std::is_integral_v<type>)
            {
                if constexpr (std::is_signed_v<type>)
                    return static_cast<type>(std::stoll(std::string(value)));
                else
                    return static_cast<type>(std::stoull(std::string(value)));
            }
            else if constexpr (std::is_floating_point_v<type>)
            {
                return static_cast<type>(std::stod(std::string(value)));
            }
            else if constexpr (std::is_same_v<type, std::string>)
            {
                return std::string(value);
            }
            else if constexpr (std::is_constructible_v<type, std::string>)
            {
                return type(std::string(value));
            }
            else
            {
                std::stringstream iss((std::string(value)));
                type result;
                if (iss >> result)
                {
                    return result;
                }
                static_assert(!sizeof(type), "Unsupported type");
            }
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("Unable to convert", value, "to required type! Using default constructed value instead!");
            // ASSERT(false, "Unable to convert", value, "to required type! Using default constructed value instead!");
        }
        return type{};
    }

    template <typename type>
    void ArgParser::option(type &store, std::string_view option_name, std::string_view description, bool required)
    {
        ASSERT(!option_name.contains('-'), "Option_name is written without '-'!");
        CLArg opt;
        opt.long_name = option_name;
        opt.type = CLArg::Type::OPTION;
        opt.description = description;
        opt.required = required;
        opt.pos = d_clargs.size();
        opt.parsed = false;

        // store how to assign value later
        opt.set_val = [&store](std::string_view value)
        {
            store = ArgParser::convert<type>(value);
        };

        d_clargs.emplace_back(std::move(opt));
    }

    template <typename type>
    void ArgParser::argument(type &store, std::string_view description)
    {
        CLArg arg;
        arg.type = CLArg::Type::ARGUMENT;
        arg.long_name = "";
        arg.description = description;
        arg.required = true;
        arg.pos = 1 + std::count_if(d_clargs.begin(), d_clargs.end(), [](CLArg const &arg)
                                    { return arg.type == CLArg::Type::ARGUMENT; });
        arg.parsed = false;

        // store how to assign value later
        arg.set_val = [&store](std::string_view value)
        {
            store = ArgParser::convert<type>(value);
        };

        d_clargs.emplace_back(std::move(arg));
    }

} // namespace Nilib

#endif