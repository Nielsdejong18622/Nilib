#include "Nilib/Logger/Log.hpp"

int main()
{
    using namespace Nilib;
    // std::ofstream logfile("Example.log");
    Nilib::Logger customlogger;
    Nilib::Logger customlogger2;

    // Matrixf A(1, 1, 2.0);
    int A = 5;
    LOG_DEBUG("MyLogger", "Test123", "Test456", A);
    LOG_DEBUG() << "MyLogger" << "allowed!\n";

    // LOG_DEBUG("LOGGER") << "disallowed!";
    LOG_DEBUG("MyLogger", "Test123", "Test456", A); // Fine.

    for (size_t i = 0; i < 15000; i++)
    {
        LOG_DEBUG() << "Loop" << i << ((i == 15000 - 1) ? '\n' : '\r');
    }

    LOG_WARNING("Commencing string test!");

    for (size_t i = 0; i < 10; i++)
    {
        std::string mystring = "mystring";
        LOG_DEBUG() << "Printing string " << i << mystring << '\r';
    }
    LOG_DEBUG() << '\n'; // Clear the last \r line. 

    LOG_DEBUG(A, 'h', "severity level", LOG_LEVEL);

    try
    {
        int a = 5;
        throw std::runtime_error("Custom exception!");
        LOG_WARNING("Dividing a", a);
    }
    catch(std::exception const &e)
    {
        LOG_ERROR(e);
    }
    

    LOG_TRACE("Trace message.");
    LOG_DEBUG("Debug message.", A);
    LOG_INFO() << "Info message\n";
    LOG_SUCCESS("MyLogger", "Success message");
    LOG_ERROR("MyLogger", "Error message");
    LOG_PROGRESS("MyLogger", "Progress message");

    LOG_SUCCESS("Completed Logger example!");
    return 0;  
}