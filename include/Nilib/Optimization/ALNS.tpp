
template <typename Solution>
Solver<Solution>::Solver(Params const &params,
                         Solution const &initial,
                         std::vector<Solver<Solution>::Operator> const &operators,
                         std::vector<Solver<Solution>::deltaOperator> const &doperators)
    : bestfound(initial), incumbent(initial), params(params),
      d_operators(operators), d_delta_operators(doperators),
      d_callback_global_improvement(nullptr), d_callback_iteration(nullptr)
{
    ASSERT(operators.size() > 0, "ALNS::Solver requires atleast one operator!");
    LOG_PROGRESS("ALNS::Solver Version 0.10 build [Win32]");
}

// Sets initial solution.
template <typename Solution>
void Solver<Solution>::initial(Solution const &initial)
{
    bestfound = initial;
    incumbent = initial;
}

template <typename Solution>
ALNSHistory Solver<Solution>::solve()
{
    if (params.max_threads == 0)
        params.max_threads = std::max(std::thread::hardware_concurrency(), 1u);
    size_t const num_threads = params.max_threads;

    // Print starting information.
    LOG_PROGRESS("ALNS::Solver User supplied {} operators and {} delta operators", d_operators.size(), d_delta_operators.size());
    LOG_PROGRESS("ALNS::Solver Optimizing a solution of {} bytes.", sizeof(Solution));
    LOG_PROGRESS("ALNS::Solver Thread count: {} hardware supports {} concurrent threads.", num_threads, std::thread::hardware_concurrency());
    LOG_PROGRESS("ALNS::Solver Stopping conditions: Max iterations: {} ({}  without improvement) Max duration: {} ",
                 params.max_iterations, params.max_iterations_without_improvement, params.max_duration);

    float const best_obj = bestfound.objective();
    bool const init_feas = bestfound.feasible();
    LOG_PROGRESS("ALNS::Solver Initial solution objective: {} (feasible {})", best_obj, init_feas);

    std::mutex global_mutex;

    Solution global_best = bestfound;
    float global_best_obj = bestfound.objective();

    ALNSHistory global_history;
    global_history.reserve(params.max_iterations);

    if (!global_best.feasible() and !params.allow_infeasible)
    {
        LOG_ERROR("ALNS::Solver Initial solution is infeasible and ALNS solver may not explore infeasible space!");
        return global_history;
    }

    // Single-threaded.
    if (num_threads <= 1)
    {
        global_history = solve_internal(0, std::ref(global_best), std::ref(global_best_obj), std::ref(global_mutex));
    }
    // Multi-threaded.
    else
    {
        std::vector<std::future<ALNSHistory>> futures;

        for (size_t t = 0; t < num_threads; ++t)
        {
            futures.emplace_back(
                std::async(
                    std::launch::async,
                    &Solver<Solution>::solve_internal,
                    this,
                    t,
                    std::ref(global_best),
                    std::ref(global_best_obj),
                    std::ref(global_mutex)));
        }

        for (auto &f : futures)
            global_history += f.get();
    }

    if (params.history_filename != "")
    {
        LOG_PROGRESS("ALNS::Solver Writing ALNS history to file {}", params.history_filename.string());
        global_history.save(params.history_filename);
    }
    bestfound = global_best;
    incumbent = global_best;

    LOG_SUCCESS("ALNS::Solver Best found objective: {} (feasible {})", global_best_obj, bestfound.feasible());
    return global_history;
}

template <typename Solution>
float Solver<Solution>::completion(size_t iteration, size_t iterations_without_improvement, std::chrono::milliseconds const running_time)
{
    // Returns the percentage that this run is finished.
    float percentage1 = static_cast<float>(iteration) / static_cast<float>(params.max_iterations);
    float percentage2 = static_cast<float>(iterations_without_improvement) / static_cast<float>(params.max_iterations_without_improvement);
    float percentage3 = static_cast<float>(running_time.count()) / static_cast<float>(params.max_duration.count());
    return std::max({percentage1, percentage2, percentage3});
}

template <typename Solution>
float Solver<Solution>::anneal_probability(float delta, float completion_perc) const
{
    if (delta <= 0.0)
        return 1.0;

    float temperature = std::pow(0.01f, completion_perc);
    return std::exp(-10.0f * delta / temperature);
}

template <typename Solution>
ALNSHistory Solver<Solution>::solve_internal(size_t thread_id, Solution &global_best, float &global_best_obj, std::mutex &global_mutex)
{
    RNG::seed(thread_id);
    // Thread-local copies
    Solution incumbent_local = bestfound;
    Solution best_local = bestfound;
    Solution previous = bestfound;

    float best_obj = global_best_obj;
    float current_obj = best_obj;
    float violation = incumbent_local.violation();
    float const eps = 10e-06;
    size_t const iterations = params.max_iterations;

    size_t iteration_without_improv = 0;

    Nilib::Timer timer;
    ALNSHistory history;
    history.reserve(iterations);

    // IMPORTANT: ensure RNG is thread-local
    // Ideally Nilib::RNG internally uses thread_local
    size_t iteration = 0;
    for (; iteration < iterations; ++iteration)
    {
        // How far we are (%) to complete this run.
        float const complete_perc = completion(iteration, iteration_without_improv, timer.getMilliseconds());
        if (complete_perc >= 1.0)
            break;

        previous = incumbent_local;

        size_t selected_operator_idx = Nilib::RNG::index(d_operators.size());

        // In case we select a standard operator.
        Operator op = d_operators[selected_operator_idx];

        op(incumbent_local);

        float next_obj = incumbent_local.objective();
        violation = incumbent_local.violation();

        float delta = (next_obj - current_obj) / current_obj;
        float anneal_prob = anneal_probability(delta, complete_perc);

        // Log this iteration.
        if (params.history_filename != "")
            history.recordIteration(thread_id, selected_operator_idx, current_obj, violation, timer.getMilliseconds(), anneal_prob);

        if (violation > 0 and !params.allow_infeasible)
        {
            // Revert back to previous feasible solution.
            incumbent_local = previous;
            continue;
        }

        // Always accept locally improving solutions.
        bool accept = false;
        if (delta <= 0)
            accept = true;
        else
        {
            // float prob = std::exp(-5.0f * delta / temperature);
            float prob = Nilib::RNG::prob();

            accept = params.use_annealing and (prob < anneal_prob);
        }

        if (accept)
        {
            current_obj = next_obj;

            // We found a (thread-local) improving solution!
            if (next_obj < best_obj)
            {
                best_obj = next_obj;
                best_local = incumbent_local;
                iteration_without_improv = 0;

                // Update global best safely
                {

                    std::lock_guard<std::mutex> lock(global_mutex);

                    if (best_obj < global_best_obj - eps)
                    {
                        LOG_PROGRESS("ALNS::Solver Thread {} found global improvement on iteration {}. Best objective: {}",
                                     thread_id, iteration, best_obj);
                        global_best_obj = best_obj;
                        global_best = best_local;
                    }
                }
            }
        }
        // else if (Nilib::RNG::prob() < params.reset_prob)
        // {
        //     std::lock_guard<std::mutex> lock(global_mutex);
        //     incumbent_local = global_best;
        // }
        else
        {
            incumbent_local = previous;
        }

        iteration_without_improv++;
    }

    // Always synchronize with main.
    std::lock_guard<std::mutex> lock(global_mutex);
    if (best_obj < global_best_obj)
    {
        global_best_obj = best_obj;
        global_best = best_local;
    }

    LOG_PROGRESS("ALNS::Solver Thread {} performed {} iterations in {}. Best objective: {}",
                 thread_id, iteration, timer.getMilliseconds(), best_obj);

    return history;
}

// This userdefined callback is called at the start of each ALNS iteration.
template <typename Solution>
void Solver<Solution>::setIterationCallback(Solver<Solution>::Callback callback)
{
    d_callback_iteration = callback;
}

// This userdefined callback is called when a new bestfound solution is obtained.
template <typename Solution>
void Solver<Solution>::setGlobalImprovementCallBack(Solver<Solution>::Callback callback)
{
    d_callback_global_improvement = callback;
}
