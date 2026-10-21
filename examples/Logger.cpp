#include "Nilib/Logger/Log.hpp"
#include "Nilib/Core/Profiler.hpp"

#include <print>
#include <fstream>

int main()
{
    PROFILE_SCOPE("Logging Example");
    using namespace Nilib;
    std::ofstream logfile("Example.log");

    Nilib::Logger::set_color_mode(Nilib::ColorMode::Auto);

    // Matrixf A(1, 1, 2.0);
    int A = 5;
    LOG_DEBUG("MyLogger Test123 Test456 {}", A); // <- Fine.
    // LOG_DEBUG("MyLogger", "Test123", "Test456 {}", A); // <- Not okay
    // LOG_DEBUG() << "MyLogger" << "disallowed!\n";

    // LOG_DEBUG("LOGGER") << "disallowed!";
    // LOG_DEBUG("MyLogger", "Test123", "Test456", A); // Fine.

    for (size_t i = 0; i < 15000; i++)
    {
        // LOG_PROGRESS("Loop {}", i);
        Nilib::Logger::log(LogLevel::Info, i == 15000 - 1, "Loop {}", i + 1);
    }

    LOG_WARNING("Commencing string test!");
    for (size_t i = 0; i < 10; i++)
    {
        std::string mystring = std::format("Test string {}", i);

        LOG_DEBUG("{}", mystring); // Legal, but weird.
        LOG_DEBUG(mystring); // More concise.
    }
    // LOG_DEBUG() << '\n'; // Clear the last \r line.

    // LOG_DEBUG(A, 'h', "severity level", LOG_LEVEL);

    try
    {
        int a = 5;
        throw std::runtime_error("Custom exception!");
        LOG_WARNING("Dividing a {}", a);
    }
    catch (std::exception const &e)
    {
        // LOG_ERROR(e);
    }
    std::vector<int> myvec(5);

    // LOG_DEBUG("Range: {}", Logger::range(myvec));
    // LOG_DEBUG(Logger::range(myvec));              // <- Does also work.
    // LOG_DEBUG("Myvec: {}", Logger::range(myvec)); // <- Does also work.
    LOG_DEBUG("Myvec: {}", myvec); // <- Does also work.

    LOG_TRACE("Trace Pi {:.3f}", 3.141592);
    LOG_TRACE("Trace message.");
    LOG_DEBUG("Debug message. {}", A);
    // LOG_INFO() << "Info message\n";
    LOG_SUCCESS("MyLogger success message");
    LOG_ERROR("MyLogger Error message");
    LOG_PROGRESS("MyLogger Progress message");

    Nilib::Logger::set_stream(logfile);

    LOG_SUCCESS("Completed Logger example!");
    Nilib::Logger::set_stream(std::cout);

    LOG_SUCCESS("Completed Logger example!");
    return 0;
}