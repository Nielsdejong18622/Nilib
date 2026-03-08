#include "Nilib/Logger/CSV.hpp"

int main()
{
    int a = 5;
    int b, c;
    Nilib::CSVWriter writer("test.csv");                // Opens immediately.
    Nilib::CSVWriter iteration_writer("iteration.csv"); // Opens immediately.
    // First call will insert the header " a, b, c" exactly as typed on line 7, and the values of the variables.
    CSV(writer, a, b, c);

    // Subsequent calls will dump the variables into the respective columns.
    CSV(writer, a, b, c);
    CSV(writer, a, b, c);

    // Very handy when you want to do this:
    for (size_t iteration = 0; iteration < 10; ++iteration)
    {
        size_t iteration2 = iteration * iteration;
        CSV(iteration_writer, iteration, iteration2);
    }

    LOG_SUCCESS("CSV example complete! Wrote to", writer.file_name());
}