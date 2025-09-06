#ifndef ARGS_H
#define ARGS_H

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Core/Enum.hpp"
#include "Nilib/Core/Assert.hpp"

#include <vector>
#include <algorithm>
#include <iomanip>

namespace Nilib
{
    class Argparser
    {
    public:
        Argparser(int argc, char **argv)
        {
            parse(argc, argv);
        }

        Argparser(std::string const &program_description, int argc, char **argv)
            : d_program_descr(program_description)
        {
            parse(argc, argv);
        }

        ~Argparser()
        {
            if (!d_checked)
            {
                LOG_WARNING("Do not forget to check the ArgParser!");
                check();
            }
        }

        inline std::string const programName() const
        {
            auto pos = d_commandline.begin()->data.find_last_of("\\");
            if (pos + 1 != std::string::npos)
                return d_commandline.begin()->data.substr(pos + 1);
            return d_commandline.begin()->data;
        }

        // Check if parsing went well. Throws error if not!
        void check()
        {
            d_checked = true;

            // // Look for "-h" or "--help"
            CommandLineArg hstr("-h");
            CommandLineArg helpstr("--help");
            if (contains(d_commandline, hstr) | contains(d_commandline, helpstr))
            {
                help();
                return;
            }

            size_t user_parsed = std::count_if(d_user.begin(), d_user.end(), [](UserArg const &arg)
                                               { return arg.parsed; });
            size_t command_parsed = std::count_if(d_commandline.begin(), d_commandline.end(), [](CommandLineArg const &arg)
                                                  { return arg.parsed; });

            LOG_DEBUG("Check: userArgs:", user_parsed, '/', d_user.size(), "commandLineArgs:", command_parsed, '/', d_commandline.size());
            if (command_parsed < d_commandline.size())
            {
                for (auto &&c_arg : d_commandline)
                {
                    if (!c_arg.parsed)
                    {
                        if (std::count(d_commandline.begin(), d_commandline.end(), c_arg) > 1)
                        {
                            LOG_ERROR("Duplicate command line argument:", c_arg.data);
                        }
                        else if (c_arg.data.size() <= 3)
                        {
                            LOG_ERROR("Unrecognized command line option:", c_arg.data);
                        }
                        // else if (c_arg.data.starts_with("-"))
                        // {
                        //     LOG_ERROR("Unrecognized command line argument:", c_arg.data, "did you mean", '-' + c_arg.data);
                        // }
                        else
                        {
                            LOG_ERROR("Unrecognized command line argument:", c_arg.data);
                        }
                    }
                }
                help();
                fail();
                return;
            }
            size_t missing = 0;
            for (auto &&user_arg : d_user)
            {
                if (user_arg.required && !user_arg.parsed)
                {
                    missing++;
                    LOG_ERROR("Missing command line argument:", user_arg.arg_desc);
                }
            }
            if (missing > 0)
            {

                help();
                fail();
            }
        }

        // template <typename Type>
        // void required_argument(Type &store, char const *arg_desc, char const *description)
        // {
        //     argument<Type>(store, '\0', arg_desc, description, true);
        // }

        template <typename Type>
        void argument(Type &store, std::string_view argc_name, std::string_view const description)
        {
            argument_interal<Type>(store, argc_name, description, false);
        }

        bool option(bool &store, std::string_view arg_desc, std::string_view description, char const arg_short = '\0')
        {
            option_internal(store, arg_desc, description, arg_short);
            return store;
        }

        // Displays the help (-h) command.
        void help() const
        {
            std::stringstream required;
            const int indentWidth = 16; // Adjust based on longest arg name

            // Print usage.
            for (auto &&userArg : d_user)
            {
                if (userArg.required)
                    required << ' ' << userArg.arg_desc << ' ';
            }
            LOG_PROGRESS(" Usage:");
            if (!required.str().empty())
                LOG_PROGRESS(" ", programName(), "[Options]", required.str(), "[Arguments]");
            else
                LOG_PROGRESS(" ", programName(), "[Options] [Arguments]");
            LOG_PROGRESS(' ');
            if (!d_program_descr.empty())
            {
                LOG_PROGRESS(d_program_descr);
                LOG_PROGRESS(' ');
            }

            LOG_PROGRESS(" Arguments:");
            for (auto &&userArg : d_user)
            {
                if (!userArg.option)
                {
                    LOG_PROGRESS() << "  "
                                   << std::left << std::setw(indentWidth)
                                   << ("--" + userArg.arg_desc)
                                   << ((userArg.arg_short == '\0') ? " \t" : (std::string("[-") + userArg.arg_short + "]\t"))
                                   << userArg.description << '\n';
                }
            }

            LOG_PROGRESS(" ");
            LOG_PROGRESS(" Options:");
            for (auto &&userArg : d_user)
            {
                if (userArg.option)
                {
                    LOG_PROGRESS() << "  "
                                   << std::left << std::setw(indentWidth)
                                   << ("--" + userArg.arg_desc)
                                   << ((userArg.arg_short == '\0') ? " \t" : (std::string("[-") + userArg.arg_short + "]\t"))
                                   << userArg.description << '\n';
                }
            }

            LOG_PROGRESS() << "  "
                           << std::left << std::setw(indentWidth)
                           << "--help"
                           << "[-h]\tShows this menu.\n";
        };

    private:
        struct UserArg
        {
            std::string arg_desc, description;
            char arg_short;
            bool parsed;
            bool option;
            bool required;
        };

        struct CommandLineArg
        {
            std::string data;
            bool parsed;

            CommandLineArg(std::string const &data) : data(data), parsed(false) {}

            operator std::string &() { return data; } // Implicit conversion.
            friend bool operator==(CommandLineArg const &self, CommandLineArg const &other) { return self.data == other.data; }
        };

        std::vector<CommandLineArg> d_commandline;
        std::vector<UserArg> d_user;
        std::string d_program_descr;
        bool d_checked;

        // Utility functions.
        template <typename Type>
        size_t find(std::vector<Type> const &search, Type const &key)
        {
            return std::distance(std::begin(search), std::find(std::begin(search), std::end(search), key));
        }

        size_t find_substr(std::vector<CommandLineArg> const &search, std::string const &prefix, std::vector<CommandLineArg>::const_iterator const &start)
        {
            auto pred = [&prefix](CommandLineArg const &type)
            { return type.data.starts_with(prefix); };
            return std::distance(std::begin(search), std::find_if(start, std::end(search), pred));
        }

        template <typename Type>
        bool contains(std::vector<Type> const &search, Type const &key)
        {
            return find(search, key) != search.size();
        }

        void parse(int argc, char **argv)
        {
            d_commandline.reserve(argc);
            for (int i = 0; i < argc; i++)
            {
                d_commandline.emplace_back(argv[i]);
            }
            d_commandline[0].parsed = true;
        }

        void fail()
        {
            std::exit(EXIT_FAILURE);
            throw std::runtime_error{std::string("Failure while parsing") + programName()};
        }

        template <typename Type>
        bool equality_arg(CommandLineArg const &shorthand_eq, Type &store)
        {
            size_t short_it = find_substr(d_commandline, shorthand_eq.data, d_commandline.begin());
            if (short_it < d_commandline.size())
            {
                // If it was already parsed, the symbol has been used before.
                if (d_commandline[short_it].parsed)
                {
                    LOG_ERROR("Argument", shorthand_eq.data, "is supplied more than once!");
                    fail();
                    return false;
                }
                d_commandline[short_it].parsed = true;

                // Otherwise we parse it.
                std::string value = d_commandline[short_it].data.substr(shorthand_eq.data.size());
                std::stringstream convert(value);
                try
                {
                    if (!(convert >> store) || !(convert >> std::ws).eof())
                    {
                        LOG_ERROR("Failed parsing of argument:", d_commandline[short_it].data, value);
                        fail();
                        return false;
                    }
                    d_user.back().parsed = true;
                }
                catch (const std::exception &e)
                {
                    LOG_ERROR("Failed parsing of argument:", d_commandline[short_it].data, value);
                    fail();
                    return false;
                }
            }
            return true;
        }

        template <typename Type>
        bool space_arg(CommandLineArg const &arg, Type &store)
        {
            if (contains(d_commandline, arg))
            {
                size_t short_it = find(d_commandline, arg);
                // If it was already parsed, the symbol has been used before.
                if (d_commandline[short_it].parsed)
                {
                    LOG_ERROR("Argument", arg.data, "is supplied more than once!");
                    fail();
                    return false;
                }
                d_commandline[short_it].parsed = true;

                // This must be the value of the argument.
                size_t val_it = short_it + 1;
                if (val_it >= d_commandline.size())
                {
                    LOG_ERROR("Expected argument value following", arg.data);
                    fail();
                    return false;
                }
                if (d_commandline[val_it].data == "-h" || d_commandline[val_it].data == "--help")
                {
                    return false;
                }
                else if (d_commandline[val_it].data.starts_with('-'))
                {
                    LOG_ERROR("Expected argument value following", arg.data, "but got", d_commandline[val_it].data);
                    fail();
                    return false;
                }
                d_commandline[val_it].parsed = true;
                try
                {
                    // Otherwise we parse it.
                    std::stringstream convert(d_commandline[val_it].data);
                    if (!(convert >> store) || !(convert >> std::ws).eof())
                    {
                        LOG_ERROR("Failed parsing of argument:", arg.data, d_commandline[val_it].data);
                        fail();
                        return false;
                    }
                    d_user.back().parsed = true;
                }
                catch (const std::exception &e)
                {
                    LOG_ERROR("Failed parsing of argument:", arg.data, d_commandline[val_it].data);
                    fail();
                    return false;
                }
            }
            return true;
        }
        // If the option is found. Toggle store.
        void option_internal(bool &store, std::string_view arg_desc, std::string_view description, char arg_short = '\0')
        {
            UserArg arg;
            arg.arg_desc = arg_desc;
            arg.arg_short = arg_short;
            arg.description = description;
            arg.required = false;
            arg.parsed = false;
            arg.option = true;

            CommandLineArg const longhand = std::string("--") + std::string(arg_desc);
            size_t command2_id = find(d_commandline, longhand);
            CommandLineArg const shorthand = std::string("-") + arg_short;
            size_t command1_id = find(d_commandline, shorthand);
            if (command1_id < d_commandline.size())
            {
                d_commandline[command1_id].parsed = true;
                arg.parsed = true;
                store = !store;
            }
            else if (command2_id < d_commandline.size())
            {
                d_commandline[command2_id].parsed = true;
                arg.parsed = true;
                store = !store;
            }
            d_user.push_back(arg);
        }

        template <typename Type>
        void argument_interal(Type &store, std::string_view arg_desc, std::string_view description, bool const required = false, char arg_short = '\0')
        {
            UserArg arg;
            arg.arg_desc = arg_desc;
            arg.arg_short = arg_short;
            arg.description = description;
            arg.required = required;
            arg.parsed = false;
            arg.option = false;
            d_user.push_back(arg);

            CORE_ASSERT(arg.arg_desc != "help");
            CORE_ASSERT(arg.arg_short != 'h');

            // Search for shorthand space, e.g. '-i myfile.csv'
            CommandLineArg const shorthand = std::string("-") + arg_short;
            if (!space_arg<Type>(shorthand, store))
                return;

            // Search for command lines starting with shorthand_eq, e.g. '-i='
            CommandLineArg const shorthand_eq = std::string("-") + arg_short + '=';
            if (!equality_arg<Type>(shorthand_eq, store))
                return;

            // Search for shorthand space, e.g. '--inputfile myfile.csv'
            CommandLineArg const arg_space = std::string("--") + std::string(arg_desc);
            if (!space_arg<Type>(arg_space, store))
                return;

            // Search for command lines starting with shorthand_eq, e.g. '-i='
            CommandLineArg const arg_eq = std::string("--") + std::string(arg_desc) + '=';
            if (!equality_arg<Type>(arg_eq, store))
                return;
        }
    };

} // namespace Nilib

#endif