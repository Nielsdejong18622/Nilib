#ifndef _ALNS_HPP
#define _ALNS_HPP

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Logger/CSV.hpp"
#include "Nilib/Core/Assert.hpp"
#include "Nilib/Core/Profiler.hpp"
#include "Nilib/Math/RNG.hpp"

#include <vector>
#include <memory>
#include <functional>
#include <filesystem>
#include <thread>

namespace Nilib
{

    namespace ALNS
    {

        class Solution
        {
            virtual float objective() const = 0;
            virtual bool feasible() const = 0;
        };

        struct Params
        {
            // Termination conditions.
            size_t max_iterations = 10'000;
            size_t max_iterations_without_improvement = 2'000;
            std::chrono::nanoseconds max_duration;

            // Allow the ALNS solver to traverse infeasible solution space.
            bool allow_infeasible = false;

            // If enabled we log to this file.
            std::filesystem::path history_filename;

            // Threads.
            size_t num_threads = 1;
        };

        // Standard ALNS framework procedure.
        template <typename Solution>
        class Solver
        {
        public:
            using Operator = void (*)(Solution &);
            using Callback = std::function<void(Solution &)>;

            Solution bestfound;
            Solution incumbent;
            Params params;

            Solver() = delete;
            Solver(Params const &params, std::vector<Operator> const &operators, Solution const &initial)
                : bestfound(initial),
                  incumbent(initial),
                  params(params),
                  d_operators(operators),
                  d_logger(params.history_filename),
                  d_callback_global_improvement(nullptr),
                  d_callback_iteration(nullptr)
            {
                ASSERT(operators.size() > 0, "ALNS::Solver requires atleast one operator!");
                LOG_PROGRESS("ALNS::Solver Version 0.10 build [Win32]");
            }

            // Sets initial solution.
            void initial(Solution const &initial)
            {
                bestfound = initial;
                incumbent = initial;
            }

            void solve()
            {
                size_t const num_threads = params.num_threads;

                // Print starting information.
                LOG_PROGRESS("User supplied", d_operators.size(), "operators");
                LOG_PROGRESS("Optimizing a solution of size ", sizeof(Solution), "bytes");
                LOG_PROGRESS("Thread count:", num_threads);

                float const best_obj = bestfound.objective();
                LOG_PROGRESS("Initial solution objective:", best_obj);

                if (num_threads <= 1)
                {
                    solve_single_thread();
                    return;
                }

                std::mutex global_mutex;

                Solution global_best = bestfound;
                float global_best_obj = bestfound.objective();

                std::vector<std::thread> threads;

                for (size_t t = 0; t < num_threads; ++t)
                {
                    threads.emplace_back(
                        &Solver<Solution>::worker,
                        this,
                        t,
                        params.max_iterations,
                        std::ref(global_best),
                        std::ref(global_best_obj),
                        std::ref(global_mutex));
                }

                for (auto &t : threads)
                    t.join();

                bestfound = global_best;
                incumbent = global_best;

                LOG_PROGRESS("Best objective:", global_best_obj);
            }

            // Solve using ALNS.
            void solve_single_thread()
            {
                // Parameter
                size_t const num_threads = params.num_threads;

                incumbent = bestfound;

                float best_obj = bestfound.objective();
                float current_obj = best_obj;
                float temperature = 1.0f;

                if (params.history_filename != "")
                {
                    d_logger.reset();
                    d_logger << "iteration,best_obj,current_obj,delta,current_feasible,next_obj,next_feasible,temperature,selected_operator_idx\n";
                    LOG_PROGRESS("Writing history to file", params.history_filename.string());
                }

                // Cooling rate so that acceptance probability is ~0.01 at the end
                float const coolingrate = std::pow(0.01f, 1.0f / params.max_iterations);

                Nilib::Timer timer;
                bool next_feasible = incumbent.feasible();
                size_t iteration = 0;
                size_t iteration_without_improv = 0;
                for (; iteration < params.max_iterations; ++iteration)
                {

                    if (timer.getNanoseconds() > params.max_duration)
                        break;
                    if (iteration_without_improv >= params.max_iterations_without_improvement)
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
                    float delta = (next_obj - current_obj) / current_obj;

                    // If we can not traverse infeasible solution space and the next solution is infeasible.
                    if (!params.allow_infeasible && next_feasible == false)
                    {
                        incumbent = previous;
                    }
                    // If the next solution is okay to go.
                    else
                    {
                        // Delta cost (new - current)

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
                                iteration_without_improv = 0;
                                // LOG_PROGRESS("Found global improvement, objective:", best_obj);
                            }
                        }
                        else
                        {
                            // Revert the change
                            incumbent = bestfound;
                        }
                    }
                    iteration_without_improv++;

                    if (params.history_filename != "")
                    {
                        // TODO: CSV does not work when called multiple times as we have static.
                        // Thus we require LOG_PROGRESS_TO(history_filename, ... ) with header inserted earlier.
                        // CSV(params.history_filename.c_str(), iteration, best_obj, current_obj, delta, current_feasible, next_obj, next_feasible, temperature, selected_operator_idx);
                        char const sep = ',';
                        d_logger << iteration << sep << best_obj << sep << current_obj << sep << delta << sep << current_feasible << sep << next_obj << sep << next_feasible << sep << temperature << sep << selected_operator_idx << '\n';
                    }

                    // Cool down
                    temperature *= coolingrate;
                }

                LOG_PROGRESS("ALNS solver performed", iteration, "iterations in", timer.getMilliseconds());
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

            constexpr void nullCallback(Solution &solution) {};

            void worker(size_t thread_id,
                        size_t iterations,
                        Solution &global_best,
                        float &global_best_obj,
                        std::mutex &global_mutex)
            {
                // Thread-local copies
                Solution incumbent_local = bestfound;
                Solution best_local = bestfound;

                float best_obj = best_local.objective();
                float current_obj = best_obj;
                float temperature = 1.0f;

                float const coolingrate =
                    std::pow(0.01f, 1.0f / iterations);

                bool next_feasible = incumbent_local.feasible();
                size_t iteration_without_improv = 0;

                Nilib::Timer timer;

                // IMPORTANT: ensure RNG is thread-local
                // Ideally Nilib::RNG internally uses thread_local

                for (size_t iteration = 0; iteration < iterations; ++iteration)
                {
                    if (timer.getNanoseconds() > params.max_duration)
                        break;

                    if (iteration_without_improv >=
                        params.max_iterations_without_improvement)
                        break;

                    Solution previous = incumbent_local;

                    size_t selected_operator_idx =
                        Nilib::RNG::index(d_operators.size());

                    Operator op = d_operators[selected_operator_idx];

                    bool current_feasible = next_feasible;
                    op(incumbent_local);

                    float next_obj = incumbent_local.objective();
                    next_feasible = incumbent_local.feasible();

                    float delta =
                        (next_obj - current_obj) / current_obj;

                    if (!params.allow_infeasible && !next_feasible)
                    {
                        incumbent_local = previous;
                    }
                    else
                    {
                        bool accept = false;

                        if (delta <= 0)
                        {
                            accept = true;
                        }
                        else
                        {
                            float prob =
                                std::exp(-5.0f * delta / temperature);

                            if (Nilib::RNG::prob() < prob)
                                accept = true;
                        }

                        if (accept)
                        {
                            current_obj = next_obj;

                            if (next_obj < best_obj)
                            {
                                best_obj = next_obj;
                                best_local = incumbent_local;
                                iteration_without_improv = 0;
                            }
                        }
                        else
                        {
                            incumbent_local = previous; // correct revert
                        }
                    }

                    iteration_without_improv++;
                    temperature *= coolingrate;
                }

                // Update global best safely
                {
                    std::lock_guard<std::mutex> lock(global_mutex);

                    if (best_obj < global_best_obj)
                    {
                        global_best_obj = best_obj;
                        global_best = best_local;
                    }

                    LOG_PROGRESS("ALNS solver thread", thread_id, "performed", iterations, "iterations in", timer.getMilliseconds(), "Best objective:", best_obj);
                }
            }
        };

    } // namespace ALNS
}; // namespace Nilib

#endif