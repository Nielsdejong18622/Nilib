#ifndef _ALNS_HPP
#define _ALNS_HPP

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Logger/CSV.hpp"
#include "Nilib/Core/Assert.hpp"
#include "Nilib/Core/Profiler.hpp"
#include "Nilib/Math/RNG.hpp"

#include <vector>
#include <memory>

namespace ALNS
{

    class Solution
    {
        virtual float objective() const = 0;
    };

    struct ALNSparams
    {
        size_t max_iterations = 1000;
        size_t max_seconds = 3;
        float starting_temperature = 100.0f;
        std::string history_filename = "";
    };

    // Standard ALNS framework procedure.
    template <typename Solution>
    class Solver
    {
    public:
        using Operator = void (*)(Solution &);
        using Callback = Operator;

        Solution bestfound;
        Solution incumbent;
        ALNSparams params;

        Solver(ALNSparams const &params, std::vector<Operator> const &operators, Solution const &initial)
            : bestfound(initial),
              incumbent(initial),
              params(params),
              d_operators(operators),
              d_logger(params.history_filename),
              d_callback_global_improvement(nullptr),
              d_callback_iteration(nullptr)
        {
            ASSERT(operators.size() > 0, "ALNS::Solver requires atleast one operator!");
        }

        void solve()
        {
            incumbent = bestfound;

            float best_obj = bestfound.objective();
            float current_obj = best_obj;
            float temperature = 1.0f;
            LOG_PROGRESS("ALNS Solver starting with obj:", best_obj, "max_iterations:", params.max_iterations);
            if (params.history_filename != "")
                LOG_PROGRESS("ALNS Solver writing history to file", params.history_filename);

            // Cooling rate so that acceptance probability is ~0.01 at the end
            float const coolingrate = std::pow(0.001f, 1.0f / params.max_iterations);
            Nilib::Timer timer;

            size_t iteration = 0;
            for (; iteration < params.max_iterations; ++iteration)
            {
                if (timer.getSeconds() > params.max_seconds - 1)
                    break;
                // Save current solution
                auto previous = incumbent;

                // Apply a random operator
                Operator op = d_operators[Nilib::RNG::index(d_operators.size())];
                op(incumbent);

                // Objective of the new solution
                float next_obj = incumbent.objective();

                // Delta cost (new - current)
                float delta = (next_obj - current_obj) / current_obj;

                bool accept = false;
                bool accept_with_anneal = false;
                if (delta < 0)
                {
                    // Better solution, always accept
                    accept = true;
                }
                else
                {
                    // Worse solution, accept with probability
                    float prob = std::exp(-5.0f * delta / temperature);
                    accept_with_anneal = (Nilib::RNG::prob() < prob);
                }

                if (accept || accept_with_anneal)
                {
                    current_obj = next_obj;

                    if (next_obj < best_obj)
                    {
                        if (d_callback_global_improvement)
                            d_callback_global_improvement(incumbent);
                        best_obj = next_obj;
                        bestfound = incumbent;
                    }
                }
                else
                {
                    // Revert the change
                    incumbent = bestfound;
                }

                if (params.history_filename != "")
                {
                    CSV(params.history_filename.c_str(), iteration, best_obj, current_obj, next_obj, temperature);
                }

                // Cool down
                temperature *= coolingrate;
            }

            LOG_PROGRESS("ALNS solver performed", iteration, "iterations in", timer.getMilliseconds(), "ms.");
        }

        void setGlobalImprovementCallBack(Callback callback)
        {
            d_callback_global_improvement = callback;
        }

    private:
        std::vector<Operator> d_operators;
        Nilib::BasicFileLogger d_logger;

        Callback d_callback_global_improvement;
        Callback d_callback_iteration;

        void nullCallback(Solution &solution) {
            // Do nothing.
        };
    };

} // namespace ALNS

#endif