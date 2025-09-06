#include "Nilib/Core/ArgParser.hpp"

#include "Nilib/Core/Enum.hpp"

using namespace Nilib;

ENUM(Strategy, DIVIDE = 1, BENDERS = 2, CGA = 0, DEFAULT = 3);

int main(int argc, char **argv)
{
    Argparser parser("Example program that shows how to use the Argparser class.", argc, argv);

    // // Which positional arguments does this program neeed!
    std::string inputfile = "Inputfile";
    int integer;
    bool verbose = false;
    Strategy strat = Strategy::DEFAULT;

    parser.argument_interal(inputfile, 'i', "inputfile", "Inputfile for the program.");
    parser.argument_interal<int>(integer, 'd', "dim", "Dimension.");
    parser.option(verbose, 'v', "verbose", "Enables verbose printing of messages.");

    // TODO: integrate such that Strategy can also be parsed instead of explcitily converting from string to strategy.
    parser.argument<Strategy>(strat, "strategy", Strategy::description());
    parser.check();

    LOG_SUCCESS("Program", parser.programName(), "arguments:", "InputFile:", inputfile, "integerDim:", integer, "verbose:", verbose, "strategy:", std::string(strat));

    return 0;
}