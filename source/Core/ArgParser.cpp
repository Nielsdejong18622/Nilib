#include "Nilib/Core/ArgParser.hpp"
#include "Nilib/Core/Assert.hpp"
#include <algorithm>

Nilib::ArgParser::ArgParser(std::string_view program_description, std::string_view version)
    : d_clargs(), d_program_name(""), d_program_description(program_description), d_version(version), d_display_help(false), d_argc(0)
{
    flag(d_display_help, "help", "Displays the help menu", 'h');
}

void Nilib::ArgParser::flag(bool &store, std::string_view flag_name, std::string_view description, char flag_char)
{
    ASSERT(!flag_name.contains('-'), "Flag_name is written without '-'!");
    ASSERT(flag_char != '-', "Flag_char can not be -!");
    CLArg flag;
    flag.description = description;
    flag.long_name = flag_name;
    flag.short_name = flag_char;
    flag.type = CLArg::Type::FLAG;
    flag.pos = d_clargs.size();
    flag.parsed = false;
    flag.required = false;

    store = false; // Set default immediately.

    flag.set_val = [&store](std::string_view value)
    {
        store = convert<bool>(value);
    };
    d_clargs.emplace_back(std::move(flag));
}

std::string_view Nilib::ArgParser::program_name() const
{
    return d_program_name;
}

int Nilib::ArgParser::argc() const
{
    return d_argc;
}

bool Nilib::ArgParser::parse(int argc, char **argv)
{
    // Store argcount.
    d_argc = argc;
    // Process program name.
    d_program_name = std::string(argv[0]);
    if (d_program_name.contains('\\'))
    {
        size_t const pos = d_program_name.find_last_of('\\');
        d_program_name = d_program_name.substr(pos + 1, d_program_name.size());
    }

    // Sort arguments, options, flags.
    std::sort(d_clargs.begin(), d_clargs.end());

    // Match argv with d_clargs.
    bool next_arg_expected_value = false;
    std::vector<CLArg>::iterator last = d_clargs.end();

    // Iterate through the raw command line.
    for (int idx = 1; idx < argc; ++idx)
    {
        std::string arg(argv[idx]);

        CORE_ASSERT(arg.size() > 0);

        // If arg starts with.
        if (arg[0] == '-')
        {
            // Previous option must have been a flag.
            if (next_arg_expected_value)
            {
                // LOG_DEBUG("Previous argument must have been a flag!");
                CORE_ASSERT(last->type == CLArg::Type::FLAG);
                last->set_val("true");
                last->parsed = true;
                next_arg_expected_value = false;
            }

            // Long option (or flag). --verbose, --obs=4 and --obs 4
            // Short option (or flag) -verbose, -o=5 and -o 5
            // Convert --name to -name.
            if (arg.size() > 1 and arg[1] == '-')
            {
                arg = arg.substr(1, arg.size());
            }
            // We only deal with short options from now, e.g. -obs=4
            if (arg.contains('='))
            {
                // Split on '='
                std::string::iterator equal_sign = std::find(arg.begin(), arg.end(), '=');
                size_t pos = std::distance(arg.begin(), equal_sign);
                std::string name = arg.substr(1, pos - 1);
                std::string value = arg.substr(pos + 1, arg.size());

                // LOG_DEBUG("Option/flag with equality!", name, value);
                last = std::find_if(d_clargs.begin(), d_clargs.end(), [&name](CLArg const &arg)
                                    { return (arg.long_name == name or arg.short_name == name) and !arg.parsed; });
                if (last != d_clargs.end())
                {
                    last->parsed = true;
                    if (last->type == CLArg::Type::FLAG)
                    {
                        last->set_val("true");
                    }
                    else
                    {
                        last->set_val(value);
                    }
                }
                else
                {
                    // LOG_DEBUG("Unrecognized!", arg);
                    d_unrecognized.push_back(arg);
                }
                next_arg_expected_value = false;
            }
            else
            {
                // Search for long option.
                std::string name = arg.substr(1, arg.size());
                // LOG_DEBUG("Option/flag without equality!", name);

                last = std::find_if(d_clargs.begin(), d_clargs.end(), [&name](CLArg const &arg)
                                    { return (arg.long_name == name or arg.short_name == name) and !arg.parsed; });

                // If found. Value comes next ()
                if (last != d_clargs.end())
                {
                    // Value follows (if option)
                    if (last->type == CLArg::Type::FLAG)
                    {
                        last->set_val("true");
                        next_arg_expected_value = false;
                    }
                    else
                    {
                        next_arg_expected_value = true;
                    }
                }
                else
                {
                    d_unrecognized.push_back(arg);
                    next_arg_expected_value = false;
                }
            }
        }
        // This is the value of the previous CLArg. (-o 4)
        else if (next_arg_expected_value)
        {
            // LOG_DEBUG("Argument after option!", arg);
            CORE_ASSERT(last != d_clargs.end());
            last->parsed = true;
            last->set_val(arg);
            next_arg_expected_value = false;
        }
        else
        {
            // LOG_DEBUG("Standalone!");
            // This is a stand-alone positional argument.
            // Search for the first non-parsed stand-alone positional argument.
            size_t jdx = 0;
            for (; jdx < d_clargs.size(); ++jdx)
            {
                if (d_clargs[jdx].type == CLArg::Type::ARGUMENT and !d_clargs[jdx].parsed)
                {
                    // LOG_DEBUG("Positional argument!", arg);
                    d_clargs[jdx].set_val(arg);
                    d_clargs[jdx].parsed = true;
                    break;
                }
            }

            // If not matched to a positional argument, it is an unrecognized command line argument.
            if (jdx == d_clargs.size())
            {
                // LOG_DEBUG("Unrecognized argument!", arg);
                d_unrecognized.push_back(arg);
            }

            next_arg_expected_value = false;
        }
    }
    // Previous option must have been a flag.
    if (next_arg_expected_value)
    {
        // LOG_DEBUG("Last argument must have been a flag!");
        last->set_val("true");
        last->parsed = true;
    }

    // If arguments left over or argument is not succesfully parsed -> return false;
    bool const all_good = all_parsed();

    // Print help if specifically requested, or there is a parsing error.
    if (!all_good or d_display_help)
    {
        help();
        std::exit(EXIT_FAILURE);
    }
    return all_good;
}

void Nilib::ArgParser::parse_or_exit(int argc, char **argv)
{
    if (!parse(argc, argv))
    {
        // In case we have already printed (because --help was in the CL) skip double printing.
        help();
        std::exit(EXIT_FAILURE);
    }
}

// Prints which commandline arguments failed parsing.
// Returns
bool Nilib::ArgParser::all_parsed() const
{
    // Check for duplicate arguments, options or flags in user input.
    for (size_t idx = 0; idx < d_clargs.size(); ++idx)
    {
        for (size_t jdx = 0; jdx < d_clargs.size(); ++jdx)
        {
            if (idx == jdx)
                continue;
            if (!d_clargs[idx].long_name.empty() and d_clargs[idx].long_name == d_clargs[jdx].long_name)
            {
                LOG_ERROR("Duplicate long name:", d_clargs[idx].long_name);
                return false;
            }
            if (!d_clargs[idx].short_name.empty() and d_clargs[idx].short_name == d_clargs[jdx].short_name)
            {
                LOG_ERROR("Duplicate short name:", d_clargs[idx].short_name);
                return false;
            }
        }
    }
    // Loop over the user supplied arguments and count how many have been parsed.
    size_t parsed = 0;
    for (size_t idx = 0; idx < d_clargs.size(); ++idx)
    {
        if (!d_clargs[idx].parsed and d_clargs[idx].required)
        {
            if (d_clargs[idx].type == CLArg::Type::ARGUMENT)
            {
                LOG_ERROR() << "Unable to parse required argument: Arg" << d_clargs[idx].pos << '\n';
            }
            else if (d_clargs[idx].type == CLArg::Type::OPTION)
            {
                LOG_ERROR() << "Unable to parse required option: --" << d_clargs[idx].long_name << '\n';
            }
            else
            {
            }
        }
        else
        {
            parsed += 1;
        }
    }

    // Loop over remaining command line arguments that were not recognized.
    if (!d_unrecognized.empty())
    {
        for (auto &&unreq : d_unrecognized)
        {
            LOG_ERROR() << "Unable to parse input: " << unreq << '\n';
        }
        return false;
    }

    return (parsed == d_clargs.size());
}

void Nilib::ArgParser::help() const
{
    std::stringstream usage_str;
    // uint16_t const indentWidth = 16; // Adjust based on longest arg name
    uint16_t arg_count = 0;         // Adjust based on longest arg name
    uint16_t flag_count = 0;        // Adjust based on longest arg name
    uint16_t options_count = 0;     // Adjust based on longest arg name
    uint16_t req_options_count = 0; // Adjust based on longest arg name

    // Print overview.
    if (d_program_description != "")
    {
        LOG_PROGRESS("Overview:");
        LOG_PROGRESS() << '\t' << d_program_description << '\n';
    }
    if (d_version != "")
    {
        LOG_PROGRESS() << "\tversion: " << d_version << '\n';
        LOG_PROGRESS(' ');
    }

    // Print usage.
    LOG_PROGRESS("Usage:");
    usage_str << '\t' << program_name();
    for (auto &&userArg : d_clargs)
    {
        switch (userArg.type)
        {
        case CLArg::Type::ARGUMENT:
            if (arg_count++ == 0 and userArg.required)
                usage_str << " Arg" << arg_count;
            break;
        case CLArg::Type::OPTION:
            if (userArg.required)
            {
                usage_str << " --" << userArg.long_name << "=...";
                if (userArg.short_name != "")
                    usage_str << " [-" << userArg.short_name << ']';
                req_options_count++;
            }
            options_count++;

            break;

        case CLArg::Type::FLAG:
            flag_count++;
            /* code */
            break;
        default:
            break;
        }
    }
    if (options_count > req_options_count)
    {
        usage_str << " [extra options]";
    }
    if (flag_count > 0)
    {
        usage_str << " [flags]";
    }
    LOG_PROGRESS(usage_str.str());
    LOG_PROGRESS(' ');

    // Argument menu.
    if (arg_count > 0)
    {
        LOG_PROGRESS("Arguments:");
        uint16_t argc = 0;
        for (auto &&userArg : d_clargs)
        {
            if (userArg.type == CLArg::Type::ARGUMENT)
            {
                LOG_PROGRESS() << '\t' << "Arg" << ++argc << ((userArg.required) ? " [req]\t\t" : "      \t\t") << userArg.description << '\n';
            }
        }
        LOG_PROGRESS(' ');
    }
    if (options_count > 0)
    {
        LOG_PROGRESS("Options:");
        for (auto &&userArg : d_clargs)
        {
            if (userArg.type == CLArg::Type::OPTION)
            {
                LOG_PROGRESS() << "\t--" << userArg.long_name << ((userArg.short_name != "") ? (" -" + userArg.short_name) : "") << ((userArg.required) ? " [req]\t" : "      \t") << userArg.description << '\n';
            }
        }
        LOG_PROGRESS(' ');
    }
    if (flag_count > 0)
    {
        LOG_PROGRESS("Flags:");
        for (auto &&userArg : d_clargs)
        {
            if (userArg.type == CLArg::Type::FLAG)
            {
                LOG_PROGRESS() << "\t--" << userArg.long_name << ((userArg.short_name != "") ? (" -" + userArg.short_name) : "") << "\t\t" << userArg.description << '\n';
            }
        }
    }
}
