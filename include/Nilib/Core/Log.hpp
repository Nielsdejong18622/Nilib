#ifndef _LOGGING_H
#define _LOGGING_H

#include <ostream>
#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <mutex>

#ifdef _WIN32 // if windows.
#include <windows.h> 
#endif 




enum class LogLevel {
  Info = 0, // Default
  Success = 2, 
  Warning = 14, // WARNING
  Error = 4, // ERROR
  Debug = 8//DEBUG
};

#ifndef ISABLE_LOG 

#define DEFAULT_LOG_FILE "Last.log"


// #ifdef _WIN32 // Define an enum class for color choices
// enum class TextColor {
//     Reset = 0,  // Default color  
//     Green = 2, 
//     Yellow = 14,  
//     Red = 4,       
//     Gray = 7, 
//     Blue = 1
// };
// #endif


class Log
{
    public:    

        template<typename T>    
        Log &operator<<(T &&message) {
            //std::lock_guard<std::mutex> lock(d_mutex);
            (*d_stream) << message;
            return *this;
        }
        template<typename T> 
        Log &operator<<(T &message) {
            //std::lock_guard<std::mutex> lock(d_mutex);
            (*d_stream) << message;
            return *this;
        }

        static void stream(std::ostream &stream)
        {
            Log::info("Switching logstream to stream.\n"); 
            Log::instance().d_useColor = true;
            Log::instance().d_stream = &stream;
        }

        static void streamtoFile(char const * filename)
        {
            Log::instance().d_fstream.close(); 
            Log::info("Switching logstream to file: ") << filename << '\n';
            Log::instance().d_fstream.open(filename);
            Log::instance().d_useColor = false;
            Log::instance().d_stream = &Log::instance().d_fstream;
        }
        static void streamtoFile()
        {
            streamtoFile(DEFAULT_LOG_FILE);
        }

        // Toggle color printing (if stream allows, see return for result).
        bool color(bool const toggle)
        { 
            d_useColor = toggle and !d_stream2file; 
            (*d_stream) << "Log color:" << ((d_useColor) ? "true" : "false") << '\n';
            return d_useColor;
        }

        // Get the one and only singleton.
        static Log &instance(){
            static Log logger;
            return logger;
        }

        static Log& log(LogLevel lev = LogLevel::Info){       
            if (Log::instance().d_useColor) Log::instance().setColor(lev);
            Log::instance().insertTimeStamp();
            return Log::instance();
        }

        template<typename T>
        static Log& error(T const msg = ""){
            return log(LogLevel::Error) << msg;
        }
        
        template<typename T>
        static Log& warning(T const msg){
            return log(LogLevel::Warning) << msg;
        }
        
        template<typename T>
        static Log& info(T const msg = ""){
            return log(LogLevel::Info) << msg;
        }
        
        template<typename T>
        static Log& success(T const msg = ""){
            return log(LogLevel::Success) << msg;
        }
        
        template<typename T>
        static Log& debug(T const msg = ""){
            return log(LogLevel::Debug) << msg;
        }

        static Log& error(){
            return log(LogLevel::Error);
        }
        
        static Log& warning(){
            return log(LogLevel::Warning);
        }
        
        static Log& info(){
            return log(LogLevel::Info);
        }
        
        static Log& success(){
            return log(LogLevel::Success);
        }
        
        static Log& debug(){
            return log(LogLevel::Debug);
        }
        static void exception(std::exception &e) {
            log(LogLevel::Error) << e.what() << '\n';
        }

        static void unittest(bool const predicate, std::string const &description) {
            Log::instance().setColor(LogLevel::Info);
            Log::instance().insertTimeStamp();
            Log::instance() << "Unittest " << description << " ... ";
            Log::instance().setColor( (predicate) ? LogLevel::Success : LogLevel::Error);
            Log::instance() << ((predicate) ? "[PASS]\n" : "[FAIL]\n");
        }


    private:
        Log()
          :d_stream(&std::cerr), d_useColor(true), d_stream2file(false)
        {
            insertTimeStamp();
            openFile(DEFAULT_LOG_FILE);
            *d_stream << "Logger object constructed! Default logfile: " << DEFAULT_LOG_FILE << "\n";
        };

        ~Log()
        { 
            
            #ifdef _WIN32 
            if (d_useColor)
            {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            insertTimeStamp();
            (*d_stream) << ("Logger destroyed!") << std::flush;
            #endif 

            #ifdef __linux__ 
            if (d_useColor) (*d_stream) << "\033[0m";
            insertTimeStamp();
            (*d_stream) << "Logger destroyed!\n" << std::flush;
            #endif
            
            d_fstream.close();
        }
        
        void openFile(char const *filename)
        {
            try
            {
                d_fstream.open(filename);
            }
            catch(const std::exception& e)
            {
                (*d_stream) << e.what() << '\n';
            }
        }

        // Private members
        void insertTimeStamp()
        {
            // Print the formatted timestamp with microseconds
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();

            // Convert microseconds to a time_point with seconds precision
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
            auto time_point_seconds = std::chrono::system_clock::time_point(seconds);

            // Convert the time_point to a std::time_t
            std::time_t time_t_seconds = std::chrono::system_clock::to_time_t(time_point_seconds);

            // Convert the std::time_t to a tm structure for formatting
            std::tm* timeinfo = std::localtime(&time_t_seconds);

            // Format the timestamp as a string
            char buffer[20];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

            //auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
            //int mics = microseconds.count()  %  1000000;
            
            // Add some padding to mitigate a weird bug. 
            //char const * pad = (mics <100) ? ((mics < 10) ? "00]:" : "0]:") : "]:";

            (*d_stream) << "[" << buffer << "]:"; //<< "." << mics << pad;
        }

        // Custom manipulator to set text color of stream.
        void setColor(LogLevel level){
            if (!d_useColor) return; 
            // Set the text color based on the platform
            #ifdef _WIN32
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            if (level != LogLevel::Info) 
                SetConsoleTextAttribute(hConsole, static_cast<WORD>(level));       
            else 
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            #endif

            #ifdef __linux__
            switch (level)
            {
            case LogLevel::Error:
                (*d_stream) << "\033[31m";
                return;
            case LogLevel::Warning:
                (*d_stream) << "\033[33m";
                return;
            case LogLevel::Success:
                (*d_stream) << "\033[32m";
                return;
            case LogLevel::Debug:
                (*d_stream) << "\033[90m";
                return;
            default:
                (*d_stream) << "\033[0m";  // Reset color
                return;
            }
            #endif
        };

        

        // No copy, move or assignment. 
        Log(Log const &&log) = delete;
        Log(Log &other) = delete;
        void operator=(Log const &other) = delete; 

        // Data members.
        std::ostream *d_stream; //  Output stream to log to. (std::cerr/std::cout/file)
        bool d_useColor;
        bool d_stream2file;
        std::mutex d_mutex;
        std::ofstream d_fstream;
};


#else // Empty logging class (is optimized away by compiler)
class Log
{
public:
    struct Timer
    {
        Timer(char const*) {};
        ~Timer() {};
    };

    void instance() {};
    template<typename T> Log const &operator<<(T &) const {static Log logger;return logger;}
    template<typename T> Log const &operator<<(T &&) const {static Log logger;return logger;}

    template<typename T> static Log const &error(T) {static Log logger;return logger;}
    template<typename T> static Log const &warning(T) {static Log logger;return logger;}
    template<typename T> static Log const &success(T) {static Log logger;return logger;}
    template<typename T> static Log const &debug(T) {static Log logger;return logger;}
    template<typename T> static Log const &info(T) {static Log logger;return logger;}
    
    static Log &error() {static Log logger;return logger;};
    static Log &warning() {static Log logger;return logger;};
    static Log &success() {static Log logger;return logger;};
    static Log &debug() {static Log logger;return logger;};
    static Log &info() {static Log logger;return logger;};
    static Log &log(LogLevel = LogLevel::Debug) {static Log logger; return logger;};
    static void exception(std::exception &) {};

    void color(bool) {};
    static void streamtoFile(char const *) {};
    static void streamtoFile() {};
    static void stream(std::ostream &) {};
    
    static void unittest(bool const, std::string const &) {};

};


#endif

#endif