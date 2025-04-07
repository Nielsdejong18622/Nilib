#include "Nilib/Core/ArgParser.hpp"

using namespace Nilib;

int main(int argc, char **argv)
{
    Argparser parser(argc, argv);

    // struct Settings
    // {
    //     int integer;
    //     bool verbose;
    // } settings;

    // // Which positional arguments does this program neeed!
    std::string inputfile = parser.argument<std::string>("--inputfile", "inputfile for the program").setdefault("load.csv");
    int integer =           parser.argument<int>("--dim", "dimension");
    // auto intege2 = parser.argument<int>("outputfile").setdefault(5);

    // settings.verbose = parser.flag();

    // parser.helpinfo();

    // parser.print();

    LOG_PROGRESS("Program start!", inputfile, integer);

    return 0;
}