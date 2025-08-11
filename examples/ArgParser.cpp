#include "Nilib/Core/ArgParser.hpp"

#include "Nilib/Core/Enum.hpp"

using namespace Nilib;

int main(int argc, char **argv)
{
    ENUM(Strategy, DIVIDE = 1, BENDERS = 2, CGA = 0, DEFAULT = 3);
    
    Argparser parser(argc, argv);

    // // Which positional arguments does this program neeed!
    std::string inputfile = parser.argument<std::string>("--inputfile", "-i", "inputfile for the program");
    int integer = parser.argument<int>("--dim", "-d", "dimension");
    bool verbose = parser.option("--verbose", "-v", "verbose printing of messages");

    // TODO: integrate such that Strategy can also be parsed instead of explcitily converting from string to strategy. 
    Strategy strat = Strategy::fromName(parser.argument<std::string>("--strategy", "-strat", "Select a strategy: DIVIDE = 1, BENDERS = 2, CGA = 0, DEFAULT = 3", "DEFAULT", false));
    parser.check();

    LOG_PROGRESS("Program", parser.programName(), "arguments:", "InputFile:", inputfile, "integerDim:", integer, "verbose:", verbose, "strategy:", std::string(strat));

    return 0;
}