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
    //LOG_DEBUG("MyLogger", "Test123", "Test456", A) << 'disallowed!';
    LOG_DEBUG() << "MyLogger" << "allowed!\n";
    
    //LOG_DEBUG("LOGGER") << "disallowed!";
    
    LOG_DEBUG("MyLogger", "Test123", "Test456", A); // Fine.
    
    LOG_DEBUG_TO("MyLogger3") << "Test1232 " << "TEST 987\n"; // Allowed
    for (size_t i = 0; i < 15000; i++)
    {
        LOG_DEBUG() << "Loop" << i << ((i == 15000 - 1) ? '\n' : '\r');   
    }
    
    LOG_DEBUG(A, 'h', "severity level", LOG_LEVEL);


    LOG_TRACE("Trace message.");
    LOG_DEBUG("Debug message.", A);
    LOG_INFO() << "Info message\n";
    LOG_SUCCESS("MyLogger", "Success message");
    LOG_ERROR("MyLogger", "Error message");
    LOG_PROGRESS("MyLogger", "Progress message");

    // LoggerPool::instance().name("MyLogger2")->level(Nilib::LogLevel::Info) << "Test1" << ' ' << "Test 2\n";
    // LoggerPool::instance().global()->level(Nilib::LogLevel::Success).output("Matrix", A, "output");
    // LoggerPool::instance().name("MyLogger2")->level(Nilib::LogLevel::Success) << "Matrix " << A << " output\n";
    // LoggerPool::instance().name("MyLogger")->level(Nilib::LogLevel::Warning) << "Test1" << A << " Test 2\n";

    // // Skip the name. 
    // LoggerPool::global()->level(Nilib::LogLevel::Debug) << "Param A!";

    // The logging should be zero overhead for disabled levels. 
    //      Levels: DEBUG,
    //              INFO
    //              WARNING
    //              ERROR
    //              SUCCESS
    //              TRACE
                    

    // Log to different loggers.
    // Loggers differ in format, thread safety, file interaction and color
    //      (File logger)
    //      (Console logger)
    //      (Thread safe console logger)
    //      (Thread safe file logger)
    //      (Rotating logger)
    //      (etc ...)    

    // The macro should support inserting arbitrary types. 
    // The macro should support inserting arbitrary with closing. 
    
   return 0;
}