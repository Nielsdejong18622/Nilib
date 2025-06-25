#include "Nilib/Logger/Log.hpp"

#include "Nilib/Math/Matrix.hpp"

int main()
{
    using namespace Nilib;

    Nilib::BasicConsoleLogger customlogger(std::cout);
    Nilib::BasicFileLogger customlogger2("Log.File");

    REGISTER_LOGGER("MyLogger", &customlogger);
    REGISTER_LOGGER("MyLogger2", &customlogger2);

    Matrixf A(1, 1, {2.0});
    // LOG_DEBUG("MyLogger", "Test123", "Test456", A) << 'disallowed!';
    LOG_DEBUG() << "MyLogger" << "allowed!\n";

    // LOG_DEBUG("LOGGER") << "disallowed!";

    LOG_DEBUG("MyLogger", "Test123", "Test456", A); // Fine.

    LOG_DEBUG_TO("MyLogger3") << "Test1232 " << "TEST 987\n"; // Allowed
    for (size_t i = 0; i < 15000; i++)
    {
        LOG_DEBUG() << "Loop" << i << ((i == 15000 - 1) ? '\n' : '\r');
    }

    LOG_WARNING("Commencing string test!");

    for (size_t i = 0; i < 10; i++)
    {
        std::string mystring = "mystring";
        LOG_DEBUG() << "Printing string " << i << ": [";
        for (auto &&ch : mystring)
        {
            LOG_DEBUG() << ch << '/';
        }
        LOG_DEBUG() << ']' << '\n';
        //LOG_DEBUG() << '\n';//LOG_DEBUG(); // THIS IS IMPORTANT TO CLOSE THE STRING!
        
    }
    

    LOG_DEBUG(A, 'h', "severity level", LOG_LEVEL);

    LOG_TRACE("Trace message.");
    LOG_DEBUG("Debug message.", A);
    LOG_INFO() << "Info message\n";
    LOG_SUCCESS("MyLogger", "Success message");
    LOG_ERROR("MyLogger", "Error message");
    LOG_PROGRESS("MyLogger", "Progress message");
    
    return 0;
}