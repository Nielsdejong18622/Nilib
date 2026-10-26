#ifndef _ALNS_HPP
#define _ALNS_HPP

#include "Nilib/Core/Assert.hpp"
#include "Nilib/Core/Profiler.hpp"
#include "Nilib/Logger/CSV.hpp"
#include "Nilib/Logger/Log.hpp"
#include "Nilib/Math/RNG.hpp"

#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <future>
#include <vector>

namespace Nilib
{

    namespace ALNS
    {

        class Solution
        {
        public:
            virtual ~Solution();
            virtual float objective() const = 0;
            virtual float violation() const = 0;
            virtual bool feasible() const { return violation() <= 0.0f; };
        };

        struct Params
        {
            // Stopping conditions.
            size_t max_iterations = 100'000;
            size_t max_iterations_without_improvement = 10'000;
            std::chrono::milliseconds max_duration = std::chrono::milliseconds{1000};

            // Search settings.
            size_t max_threads = 1;
            bool allow_infeasible = false;
            bool use_annealing = true;
            float reset_prob = 0.01f; // Chance per iteration to reset to global.

            std::filesystem::path history_filename = ""; // For (optional) logging.
        };

        struct ALNSHistory
        {
            std::vector<size_t> thread_id;
            std::vector<size_t> operator_idxs;
            std::vector<float> objectives;
            std::vector<float> violations;
            std::vector<std::chrono::milliseconds> time;
            std::vector<float> anneal_prob;

            void reserve(size_t capacity)
            {
                thread_id.reserve(capacity);
                operator_idxs.reserve(capacity);
                objectives.reserve(capacity);
                violations.reserve(capacity);
                time.reserve(capacity);
                anneal_prob.reserve(capacity);
            }

            void recordIteration(size_t threadid, size_t op_idx, float obj, float violation, std::chrono::milliseconds elaps, float anneal)
            {
                thread_id.push_back(threadid);
                operator_idxs.push_back(op_idx);
                objectives.push_back(obj);
                violations.push_back(violation);
                time.push_back(elaps);
                anneal_prob.push_back(anneal);
            }

            ALNSHistory &operator+=(ALNSHistory const &other)
            {
                thread_id.insert(thread_id.begin(), other.thread_id.begin(), other.thread_id.end());
                operator_idxs.insert(operator_idxs.begin(), other.operator_idxs.begin(), other.operator_idxs.end());
                objectives.insert(objectives.begin(), other.objectives.begin(), other.objectives.end());
                violations.insert(violations.begin(), other.violations.begin(), other.violations.end());
                time.insert(time.begin(), other.time.begin(), other.time.end());
                anneal_prob.insert(anneal_prob.begin(), other.anneal_prob.begin(), other.anneal_prob.end());
                return *this;
            }

            void save(std::filesystem::path const &filepath) const
            {
                std::ofstream file(filepath, std::ios::out);
                char const sep = ',';
                file << "Thread,OperatorID,Objective,Violation,AnnealProb,Ms\n";
                for (size_t idx = 0; idx < thread_id.size(); ++idx)
                {
                    file << thread_id[idx] << sep
                         << operator_idxs[idx] << sep
                         << objectives[idx] << sep
                         << violations[idx] << sep
                         << anneal_prob[idx] << sep
                         << time[idx].count()
                         << '\n';
                }
            }
        };

        // Standard ALNS framework procedure.
        template <typename Solution>
        class Solver
        {
        public:
            using Operator = void (*)(Solution &);
            using deltaOperator = float (*)(Solution &);
            using Callback = std::function<void(Solution &)>;

            Solution bestfound;
            Solution incumbent;
            Params params;

            Solver() = delete;
            Solver(Params const &params,
                   Solution const &initial,
                   std::vector<Solver<Solution>::Operator> const &operators,
                   std::vector<Solver<Solution>::deltaOperator> const &doperators = {});

            // Sets initial solution.
            void initial(Solution const &initial);

            // Solves solution.
            ALNSHistory solve();

            // This userdefined callback is called at the start of each ALNS iteration.
            void setIterationCallback(Callback callback);

            // This userdefined callback is called when a new bestfound solution is obtained.
            void setGlobalImprovementCallBack(Callback callback);

        private:
            ALNSHistory solve_internal(size_t thread_id, Solution &global_best, float &global_best_obj, std::mutex &global_mutex);
            float completion(size_t iteration, size_t iteration_no_improv, std::chrono::milliseconds const running_time);

            float anneal_probability(float delta, float completion_perc) const;

            std::vector<Operator> d_operators;
            std::vector<deltaOperator> d_delta_operators;

            ALNSHistory history;

            Callback d_callback_global_improvement;
            Callback d_callback_iteration;

            constexpr void nullCallback(Solution const &) {};
        };
#include "ALNS.tpp"
    } // namespace ALNS
}; // namespace Nilib

#endif