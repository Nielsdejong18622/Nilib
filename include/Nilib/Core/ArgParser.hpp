#ifndef ARGS_H
#define ARGS_H

#include "Nilib/Logger/Log.hpp"

#include <vector>
#include <algorithm>

namespace Nilib
{
    class Argparser
    {

        struct Argument
        {
            std::string argument_descriptor, argument_descriptor_shorthand, description;
        };

        bool d_failed_parse;
        int d_succesfully_parsed_args;
        std::string d_program_name;
        std::vector<std::string> d_positional_argument_map;
        std::vector<Argument> d_helpmap;

        template <typename Type>
        Type getNextEnumArgument(std::string const &argument_key)
        {
            // We found the short hand, e.g. -i
            auto argument_iter = std::find(d_positional_argument_map.begin(), d_positional_argument_map.end(), argument_key);
            if (argument_iter == d_positional_argument_map.end())
            {
                LOG_ERROR("No argument following", argument_key);
                std::exit(EXIT_FAILURE);
            }

            auto argval = next(argument_iter);
            if (argval == d_positional_argument_map.end())
            {
                LOG_ERROR("No argument following", argument_key);
                std::exit(EXIT_FAILURE);
            }

            Type value = Type::fromString(*argval);
            d_succesfully_parsed_args++; // We also passed the argument here.
            return value;
        }

        template <typename Type>
        Type getNextArgument(std::string const &argument_key)
        {
            // We found the short hand, e.g. -i
            auto argument_iter = std::find(d_positional_argument_map.begin(), d_positional_argument_map.end(), argument_key);
            Type value; // Default constructed value.
            if (argument_iter == d_positional_argument_map.end())
            {
                LOG_ERROR("No argument following", argument_key);
                std::exit(EXIT_FAILURE);
                return value;
            }

            d_succesfully_parsed_args++; // We found the key.

            auto argval = next(argument_iter);
            if (argval == d_positional_argument_map.end())
            {
                LOG_ERROR("No argument following", argument_key);
                d_succesfully_parsed_args++;
                std::exit(EXIT_FAILURE);
                return value;
            }

            std::stringstream convert(*argval);
            if (!(convert >> value) || !(convert >> std::ws).eof())
            {
                LOG_ERROR("Failed parsing of argument:", *argval, "given after ", argument_key);
                return value;
            }

            d_succesfully_parsed_args++; // We also passed the argument here.
            return value;
        }

    public:
        Argparser(int argc, char **argv)
            : d_failed_parse(false),
              d_succesfully_parsed_args(1) // The program name.
        {
            parse(argc, argv);
        }

        inline std::string const &programName() { return d_program_name; }

        // Check if parsing went well. Throws error if not!
        void check()
        {
            auto &&argv = d_positional_argument_map;
            // Look for "-h" or "--help"
            if (std::find(argv.begin(), argv.end(), "-h") != argv.end() ||
                std::find(argv.begin(), argv.end(), "--help") != argv.end() ||
                std::find(argv.begin(), argv.end(), "--h") != argv.end())
            {
                helpinfo();
                std::exit(EXIT_FAILURE);
            }
            if (d_positional_argument_map.size() > d_succesfully_parsed_args)
            {
                LOG_ERROR("Received", d_positional_argument_map.size(), "arguments but succesfully parsed", d_succesfully_parsed_args, "arguments!");
                helpinfo();
                std::exit(EXIT_FAILURE);
            }
        }

        template <typename Type>
        Type argument(std::string const &argument_descriptor, std::string const &argument_shorthand, std::string const &description)
        {
            return argument<Type>(argument_descriptor, argument_shorthand, description, Type(), false);
        }

        template <typename Type>
        Type enum_argument(std::string const &argument_descriptor, std::string const &argument_shorthand, std::string const &description, Type const &defaultType, bool const required = true)
        {
            d_helpmap.emplace_back(argument_descriptor, argument_shorthand, description);

            // First search for the shorthand.
            if (std::find(d_positional_argument_map.begin(), d_positional_argument_map.end(), argument_shorthand) != d_positional_argument_map.end())
            {
                return getNextEnumArgument<Type>(argument_shorthand);
            }
            if (std::find(d_positional_argument_map.begin(), d_positional_argument_map.end(), argument_descriptor) != d_positional_argument_map.end())
            {
                return getNextEnumArgument<Type>(argument_descriptor);
            }
            if (required)
                LOG_ERROR("Program requires argument:", description);
            return defaultType;
        }

        template <typename Type>
        Type argument(std::string const &argument_descriptor, std::string const &argument_shorthand, std::string const &description, Type const &defaultType, bool const required = false)
        {

            // Log the argument in the help map.
            d_helpmap.emplace_back(argument_descriptor, argument_shorthand, description);

            // If parsing failed on a previous argument, skip this argument parsing to get to the help message.
            if (d_failed_parse)
                return Type{defaultType};

            // Parse the argument.
            // If the argument can not be located/parsed, set failbit and return default constructed value.

            // First search for the shorthand.
            if (std::find(d_positional_argument_map.begin(), d_positional_argument_map.end(), argument_shorthand) != d_positional_argument_map.end())
            {
                return getNextArgument<Type>(argument_shorthand);
            }
            if (std::find(d_positional_argument_map.begin(), d_positional_argument_map.end(), argument_descriptor) != d_positional_argument_map.end())
            {
                return getNextArgument<Type>(argument_descriptor);
            }
            if (std::find_if(d_positional_argument_map.begin(), d_positional_argument_map.end(), [&argument_shorthand](std::string const &x)
                             { return x.rfind(argument_shorthand) == 0; }) != d_positional_argument_map.end())
            {
                return parseEqualityArgument<Type>(argument_shorthand);
            }
            if (std::find_if(d_positional_argument_map.begin(), d_positional_argument_map.end(), [&argument_descriptor](std::string const &x)
                             { return x.rfind(argument_descriptor) == 0; }) != d_positional_argument_map.end())
            {
                return parseEqualityArgument<Type>(argument_descriptor);
            }
            if (required)
                LOG_ERROR("Program requires argument:", description);
            return Type{defaultType};
        }

        bool option(std::string const &argument_descriptor, std::string const &argument_descriptor_shorthand, std::string const &description)
        {
            d_helpmap.emplace_back(argument_descriptor, argument_descriptor_shorthand, description);
            // If the option (-h) is found in the argument map, it is true.
            // Otherwise it is false.
            auto &argv = d_positional_argument_map;
            bool const find_shorthand = std::find(argv.begin(), argv.end(), argument_descriptor_shorthand) != argv.end();
            bool const find_descriptor = std::find(argv.begin(), argv.end(), argument_descriptor) != argv.end();
            d_succesfully_parsed_args += find_shorthand || find_descriptor;
            return find_shorthand || find_descriptor;
        }

        template <typename Type = bool>
        bool option(std::string const &argument_descriptor, std::string const &description)
        {
            d_helpmap.emplace_back(argument_descriptor, "", description);
            // If the option (-h) is found in the argument map, it is true.
            // Otherwise it is false.
            auto &argv = d_positional_argument_map;
            bool const find_descriptor = std::find(argv.begin(), argv.end(), argument_descriptor) != argv.end();
            d_succesfully_parsed_args += find_descriptor;
            return find_descriptor;
        }

        // Prints all arguments and values.
        void print() const
        {
            LOG_DEBUG("Program:", d_program_name);
            for (auto &&v : d_positional_argument_map)
            {
                LOG_DEBUG(v);
            }
        }

        // Displays the help (-h) command.
        void helpinfo() const
        {
            LOG_PROGRESS("Usage", d_program_name);
            for (size_t argument_idx = 0; argument_idx < d_helpmap.size(); ++argument_idx)
            {
                Argument arg = d_helpmap[argument_idx];
                LOG_PROGRESS("\t", arg.argument_descriptor_shorthand, arg.argument_descriptor, arg.description);
            }

            LOG_PROGRESS("\t -h --help shows this menu");
        };

    private:
        void parse(int argc, char **argv)
        {
            // Loop through all the required positional arguments and process them.
            d_program_name = argv[0];

            d_positional_argument_map.reserve(argc);
            for (size_t i = 0; i < argc; i++)
            {
                d_positional_argument_map.emplace_back(argv[i]);
            }
        }
        template <typename Type>
        Type parseEqualityArgument(std::string const &argument_key)
        {
            auto argument = std::find_if(d_positional_argument_map.begin(), d_positional_argument_map.end(), [&argument_key](std::string const &x)
                                         { return x.rfind(argument_key) == 0; });
            Type value{0}; // Default constructed value.

            std::string argstr = *argument;
            if (argstr.length() <= argument_key.length() + 1)
            {
                // Case --inputfile=
                LOG_ERROR("Expected argument after", argstr);
                return value;
            }
            std::string arg_str = argstr.substr(argument_key.length() + 1); // length of -i=
            std::stringstream convert(arg_str);
            if (!(convert >> value) || !(convert >> std::ws).eof())
            {
                LOG_ERROR("Failed parsing of argument:", arg_str);
                return value;
            }
            d_succesfully_parsed_args++;
            return value;
        }
    };

} // namespace Nilib

/*


class Argparser
{
public:
    Argparser(char const *argdescr = "", char const *version = "1.0")
        : d_argdescr(argdescr), d_version(version)
    {
    }

    // A toggleable boolean flag. Examples -v or --v. or -verbose or --verbose
    void flag(std::string name, std::string const &descr = "")
    {
        d_flags[strtolower(name)] = false;
        d_flagdesc[strtolower(name)] = descr;
    }

    // An option like -n=40 or -n 40 or -N 40.
    void option(std::string name, std::string const &descr = "")
    {
        d_options[strtolower(name)] = std::string();
        d_optiondesc[strtolower(name)] = descr;
    }

    // Check if a flag is True or False.
    std::string const operator[](std::string &&at)
    {
        std::string lat = strtolower(at);
        if (d_options.find(lat) != d_options.end())
        {
            return d_options[lat];
        }
        return d_flags[lat] ? "True" : "False";
    }

    // Parse the command line arguments.
    void parse(int argc, char **argv)
    {
        this->argv.clear();
        this->argv.reserve(argc);

        if (argc == 1)
        {
        }
        this->argv.push_back(argv[0]);
        try
        {
            parse_internal(argc, argv);
            LOG_DEBUG() << "Done parsing all the command line arguments.\n";
            this->argc = this->argv.size();
            checkoptions();
        }
        catch (...)
        {
            // Log the help text and terminate the program.
            printHelpText();
            throw std::runtime_error("Unable to parse command line arguments!");
        }
    }

    void printHelpText()
    {
        // For every flag and option required. There is a description of correct usage.
        LOG_INFO() << "Usage: " << argv[0]
                   << " "
                   << "[options] "
                   << "[flags] "
                   << d_argdescr
                   << '\n';
        LOG_INFO() << "Options:\n";
        for (auto const &[key, val] : d_options)
        {
            LOG_INFO() << "\t-" << key << "\t\t\t" << d_optiondesc[key] << '\n';
        }

        LOG_INFO() << "Flags:\n";
        for (auto const &[key, val] : d_flags)
        {
            LOG_INFO() << "\t-" << key << "\t\t\t\t" << d_flagdesc[key] << '\n';
        }
        LOG_INFO() << argv[0] << ' ' << d_version << '\n';
        exit(0);
    }

public:
    size_t argc = 0;
    std::vector<std::string> argv;

private:
    // Remove the prefix dashes -(-)  before a string.
    std::string &removeprefixdash(std::string &str)
    {
        while (str[0] == '-')
            str.erase(0, 1);
        return str;
    }

    // Convert string to lower.
    std::string &strtolower(std::string &str)
    {
        for (auto &c : str)
            c = tolower(c);
        return str;
    }

    // Check if all required options where parsed, print those missing.
    void checkoptions()
    {
        for (auto const &[key, val] : d_options)
        {
            if (val.empty())
            {
                LOG_ERROR() << "Missing option:" << key << '\n';
                throw std::runtime_error("Expected an missing option!");
            }
        }
    }

    char const *d_argdescr;
    char const *d_version;
    std::unordered_map<std::string, std::string> d_options;
    std::unordered_map<std::string, std::string> d_optiondesc;
    std::unordered_map<std::string, bool> d_flags;
    std::unordered_map<std::string, std::string> d_flagdesc;

    // Trims withespace characters from sting.
    inline std::string &trim(std::string &str)
    {
        str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
        str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);
        return str;
    }

    void parse_internal(int argc, char **argv)
    {
        // Previous arg and bool.
        std::string prarg(argv[0]);
        bool nextoptionarg = false;

        for (int i = 1; i < argc; ++i)
        {
            std::string arg(argv[i]);
            // The previous argument was an option.
            if (nextoptionarg)
            {
                if (arg[0] == '-')
                {
                    LOG_ERROR() << '[' << arg << "] is not an argument for " << prarg << "!\n";
                    throw std::runtime_error("Incorrect argument!");
                }

                d_options[prarg] = arg;
                nextoptionarg = false;
                LOG_DEBUG() << '[' << arg << "] is the argument of " << prarg << ".\n";
                continue;
            }

            // Argument is the only remaining option.
            if (arg[0] != '-')
            {
                LOG_DEBUG() << '[' << arg << "] is an argument.\n";
                this->argv.push_back(arg);
                continue;
            }

            // if current argument starts with -(-) it is an option or a flag.
            // Convert to lower case and remove prefix dashes.
            arg = removeprefixdash(arg);
            arg = strtolower(arg);

            // Check if it is an equality option.
            if (std::find(arg.begin(), arg.end(), '=') != arg.end())
            {
                // Get option name and value (without the =) and add them to the option map.
                std::string optionName = arg.substr(0, arg.find('='));
                std::string valueName = arg.substr(arg.find('=') + 1);
                optionName = strtolower(optionName);

                LOG_DEBUG() << '[' << arg
                            << "] is an equality option, name: "
                            << optionName
                            << " value: "
                            << valueName
                            << ".\n";
                d_options[optionName] = valueName;
            }
            else if (d_options.find(arg) != d_options.end())
            {
                // Check if arg matches one of our options.
                nextoptionarg = true;
                prarg = arg;
                continue; // See first if loop.
            }
            else if (d_flags.find(arg) != d_flags.end())
            {
                // Check if it matches one of our flags.
                LOG_DEBUG() << '[' << arg << "] is a flag.\n";
                d_flags[arg] = true; // We found a flag.
            }
            else
            {
                // Nothing mathches, hence it must be an argument.
                LOG_ERROR() << '[' << arg << "] is not a flag, argument or option!\n";
                throw std::runtime_error("Processed not a flag, argument nor option!");
            }
        }
    }
};
*/
#endif