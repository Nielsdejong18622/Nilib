#ifndef ARGS_H
#define ARGS_H

#include "Nilib/Logger/Log.hpp"

namespace Nilib
{
    template <typename Type>
    struct ArgProxy
    {
        Type d_value;

        ArgProxy setdefault(Type const &value)
        {
            d_value = value;
            return *this;
        }

        // Implicit conversion.
        operator Type() const
        {
            return d_value;
        }
    };



    class Argparser
    {
        std::string d_program_name;
        std::unordered_map<int, std::string> d_positional_argument_map;

        void parse(int argc, char **argv)
        {
            LOG_DEBUG("Parsing:", argc, "arguments");
            d_program_name = argv[0];

            for (size_t i = 1; i < argc; i++)
            {
                d_positional_argument_map[i] = argv[i];
                std::string argstring = d_positional_argument_map[i];
                
                // Each command line argument is either a positional argument. file1.txt
                if (!argstring.starts_with('-'))
                {

                
                }
                // A short option -v (flag)
                else if (argstring.size() > 1 && argstring.at(1))
                {

                }

                // a long option --verbose (flag)

                // a long option --value=5 

                // a long option --out file.txt



            }
        }

    public:
        Argparser(int argc, char **argv)
        {
            parse(argc, argv);
            LOG_PROGRESS("ArgParser constructor.");
        }

        template <typename Type>
        ArgProxy<Type> argument(std::string const &argument_descriptor, std::string const &description)
        {
            LOG_PROGRESS("Program requires argument:", description, "given after", argument_descriptor);
            // // Try to find the argument in the argument_map.

            // auto result = std::find_if(
            //     d_positional_argument_map.begin(),
            //     d_positional_argument_map.end(),
            //     [val](const auto &mo)
            //     { return mo.second == val; });

            // // RETURN VARIABLE IF FOUND
            // if (result != mapObject.end())
            //     int foundkey = result->first;

            return ArgProxy<Type>();
        }

        // Prints all arguments and values.
        void print() const
        {
            LOG_DEBUG("Program:", d_program_name);
            for (auto &&[k, v] : d_positional_argument_map)
            {
                LOG_DEBUG(k, ':', v);
            }
        }

        // Displays the help (-h) command.
        void helpinfo() const
        {
            LOG_PROGRESS("Usage", d_program_name);
            LOG_PROGRESS("-h\t\tshows this info menu.");
        };
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