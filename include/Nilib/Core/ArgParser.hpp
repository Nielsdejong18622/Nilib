#ifndef ARGS_H
#define ARGS_H

#include "Nilib/Logger/Log.hpp"

#include <unordered_map>
#include <stdexcept>

/* This class passes the arguments from the terminal.
    Responsibilities:
    Exit on exception error.
    Provide help information on incorrect usage.
    Provide version info on usage.

    Set flags and options prior to parse.
    Parse the command lines.
    Supply a hasmap of options and flags.
    For other program components to use.
    Options and flags must be case insensitive
    Options and flags can have additional alternatives specified. (-n 10 same as -loCs 10 and -nLOCS=10)

    */

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

    // Parse the command line arguments.
    void parse(int argc, char const **argv)
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
            throw std::runtime_error();
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
        LOG_INFO(argv[0]) << " " << d_version << '\n';
        exit(0);
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

public:
    size_t argc = 0;
    std::vector<std::string> argv;

private:
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

    void parse_internal(int argc, char const **argv)
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
                    LOG_ERROR('[') << arg << "] is not an argument for " << prarg << "!\n";
                    throw std::runtime_error("Incorrect argument!");
                }

                d_options[prarg] = arg;
                nextoptionarg = false;
                LOG_DEBUG('[') << arg << "] is the argument of " << prarg << ".\n";
                continue;
            }

            // Argument is the only remaining option.
            if (arg[0] != '-')
            {
                LOG_DEBUG('[') << arg << "] is an argument.\n";
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

                LOG_DEBUG('[') << arg
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
                LOG_DEBUG('[') << arg << "] is a flag.\n";
                d_flags[arg] = true; // We found a flag.
            }
            else
            {
                // Nothing mathches, hence it must be an argument.
                LOG_ERROR('[') << arg << "] is not a flag, argument or option!\n";
                throw std::runtime_error("Processed not a flag, argument nor option!");
            }
        }
    }
};
#endif