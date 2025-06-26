#ifndef _ALNS_HPP
#define _ALNS_HPP

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Logger/CSV.hpp"
#include "Nilib/Core/Assert.hpp"
#include "Nilib/Core/Profiler.hpp"
#include "Nilib/Math/RNG.hpp"

#include <vector>
#include <memory>

namespace Nilib
{

    namespace ALNS
    {

        class Solution
        {
            virtual float objective() const = 0;
            virtual bool feasible() const = 0;
        };

        struct ALNSparams
        {
            size_t max_iterations = 1000;
            size_t max_seconds = 5;
            float starting_temperature = 100.0f;

            // Allow the ALNS solver to traverse infeasible solution space.
            bool allow_infeasible = false;

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

            Solver() = delete;
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

            // Sets initial solution.
            void initial(Solution const &initial)
            {
                bestfound = initial;
                incumbent = initial;
            }

            void solve()
            {
                incumbent = bestfound;

                float best_obj = bestfound.objective();
                float current_obj = best_obj;
                float temperature = 1.0f;

                // Print starting information.
                LOG_PROGRESS("ALNS::Solver Version 0.10 build [Win32]");
                LOG_PROGRESS("User supplied", d_operators.size(), "operators");
                LOG_PROGRESS("Optimizing a solution of size ", sizeof(Solution), "bytes");
                LOG_PROGRESS("Initial solution objective:", best_obj);
                if (params.history_filename != "")
                {
                    d_logger.reset();
                    LOG_PROGRESS("Writing history to file", params.history_filename);
                }

                // Cooling rate so that acceptance probability is ~0.01 at the end
                float const coolingrate = std::pow(0.01f, 1.0f / params.max_iterations);
                Nilib::Timer timer;

                bool next_feasible = incumbent.feasible();
                size_t iteration = 0;
                for (; iteration < params.max_iterations; ++iteration)
                {

                    if (timer.getSeconds() > params.max_seconds - 1)
                        break;

                    // Call user defined callback if it exists!
                    if (d_callback_iteration)
                        d_callback_iteration(incumbent);

                    // Save current solution
                    auto previous = incumbent;

                    // Apply a random operator
                    size_t const selected_operator_idx = Nilib::RNG::index(d_operators.size());
                    Operator op = d_operators[selected_operator_idx];

                    bool current_feasible = next_feasible;
                    op(incumbent);

                    // Objective of the new solution
                    float next_obj = incumbent.objective();
                    next_feasible = incumbent.feasible();

                    // If we can not traverse infeasible solution space and the next solution is infeasible.
                    if (!params.allow_infeasible && next_feasible == false)
                    {
                        incumbent = previous;
                    }
                    // If the next solution is okay to go. 
                    else
                    {
                        // Delta cost (new - current)
                        float delta = (next_obj - current_obj) / current_obj;

                        bool accept = false;
                        bool accept_with_anneal = false;

                        // Better solution, always accept
                        if (delta <= 0)
                        {
                            accept = true;
                        }
                        else
                        {
                            // Worse solution, accept with probability (simulated annealing).
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
                                // LOG_PROGRESS("Found global improvement, objective:", best_obj);
                            }
                        }
                        else
                        {
                            // Revert the change
                            incumbent = bestfound;
                        }
                    }

                    if (params.history_filename != "")
                    {
                        // TODO: CSV does not work when called multiple times as we have static. 
                        // Thus we require LOG_PROGRESS_TO(history_filename, ... ) with header inserted earlier.
                        CSV(params.history_filename.c_str(), iteration, best_obj, current_obj, current_feasible, next_obj, next_feasible, temperature, selected_operator_idx);
                    }

                    // Cool down
                    temperature *= coolingrate;
                }

                LOG_PROGRESS("ALNS solver performed", iteration, "iterations in", timer.getMilliseconds(), "ms.");
                d_logger.close();
            }

            // This userdefined callback is called at the start of each ALNS iteration.
            void setIterationCallback(Callback callback)
            {
                d_callback_iteration = callback;
            }

            // This userdefined callback is called when a new bestfound solution is obtained.
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
}; // namespace Nilib

#endif