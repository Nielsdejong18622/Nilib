#include "Nilib/Core/ArgParser.hpp"

using namespace Nilib;

int main(int argc, char **argv)
{
    Argparser parser(argc, argv);

    // // Which positional arguments does this program neeed!
    std::string inputfile = parser.argument<std::string>("--inputfile", "-i", "inputfile for the program");
    int integer =           parser.argument<int>("--dim", "-d", "dimension");
    bool verbose = parser.option<bool>("--verbose", "-v", "verbose printing of messages");
    parser.check();

    
    LOG_PROGRESS("Program", parser.programName(), "arguments:", "InputFile:", inputfile, "integerDim:", integer, "verbose:", verbose);

    return 0;
}