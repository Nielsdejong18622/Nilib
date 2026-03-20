#include "Nilib/Core/ArgParser.hpp"

#include "Nilib/Core/Enum.hpp"

using namespace Nilib;

ENUM(Strategy, DIVIDE = 1, BENDERS = 2, CGA = 0, DEFAULT = 3);

int main(int argc, char **argv)
{
    ArgParser parser("Example program that shows how to use the Argparser class.", "1.0");

    // // Which positional arguments does this program neeed!
    std::string inputfile = "Inputfile";
    int integer;
    bool verbose = false;
    Strategy strat = Strategy::DEFAULT;

    parser.option(inputfile, "inputfile", "Inputfile for the program.", true);
    parser.argument<int>(integer, "Dimension");
    parser.flag(verbose, "verbose", "Enables verbose printing of messages.", 'v');

    parser.argument<Strategy>(strat, Strategy::description());
    parser.parse_or_exit(argc, argv);

    LOG_SUCCESS("Program", parser.program_name(), "arguments:", "InputFile:", inputfile, "integerDim:", integer,
                "verbose:", verbose, "strategy:", std::string(strat));

    return 0;
}