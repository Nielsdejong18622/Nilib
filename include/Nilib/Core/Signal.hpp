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
            LOG_ERROR("Received signal:", signal);
            if (signal == SIGINT)
            {
                running = false;
            }
        }

    public:
        static Signal getInstance()
        {
            static Signal signal;
            return signal;
        }
        static bool receivedCtrlC() { return !getInstance().running; }
    };

}

#endif