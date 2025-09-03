#ifndef _CORE_SIGNAL_H
#define _CORE_SIGNAL_H

#include <atomic>
#include <csignal>
#include <functional>
#include "Nilib/Logger/Log.hpp"

namespace Nilib
{
    // Singleton.
    class Signal
    {
        static std::atomic<bool> running;
        Signal()
        {
            running = true;
            std::signal(SIGINT, Signal::signal_handler);
        }

        static void signal_handler(int signal)
        {
            if (signal == 2)
                LOG_WARNING("Received signal: SIG_KILL (CTRL-C)");
            else
                LOG_WARNING("Received signal:", signal);
            if (signal == SIGINT)
            {
                running = false;
            }
        }

        static Signal getInstance()
        {
            static Signal signal;
            return signal;
        }

    public:
        static bool receivedCtrlC() { return !getInstance().running; }
    };

}

#endif