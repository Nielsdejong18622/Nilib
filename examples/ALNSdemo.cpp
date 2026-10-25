#include "Nilib/Optimization/ALNS.hpp"

#include "Nilib/Logger/CSV.hpp"
#include "Nilib/Logger/Log.hpp"

#include "Nilib/Core/ArgParser.hpp"
#include "Nilib/Math/RNG.hpp"

struct DemoSolution : public Nilib::ALNS::Solution
{
    float x, y;

    DemoSolution(float x, float y) : x(x), y(y)
    {
    }

    float objective() const override
    {
        // Simple convex problem.
        return (x - 5.0f) * (x - 5.0f) + (y - 15.0f) * (y - 15.0f);

        // Rastrigin function.
        // return 20 + x * x + y * y - 10 * (cos(6.28 * x) + cos(6.28 * y));

        // Shaffer F6.
        // float const parabola = x * x + y * y;
        // float const denom = (1 + 0.001 * parabola);
        // return 100 * (0.5 + (sin(sqrt(parabola)) * sin(sqrt(parabola)) - 0.5) / (denom * denom));
    }

    float violation() const override
    {
        // Always feasible.
        return 0.0;
    }
};

void op1(DemoSolution &solution)
{
    solution.x += Nilib::RNG::uniform(-0.5f, 0.5f);
    solution.y += Nilib::RNG::uniform(-0.5f, 0.5f);
}

void op2(DemoSolution &solution)
{
    solution.x += Nilib::RNG::uniform(-1.0f, 1.0f);
    solution.y += Nilib::RNG::uniform(-1.0f, 1.0f);
}

void myfunction(DemoSolution &sol)
{
    LOG_DEBUG("Found global improvement: {}", sol.objective());
}

int main(int argc, char **argv)
{
    Nilib::ArgParser parser("ALNS demo example", "1.2");
    
    Nilib::RNG::seed(128);

    using namespace std::chrono_literals;

    Nilib::ALNS::Params params{
        .max_duration = 60s,
        .allow_infeasible = false,
        .use_annealing = true,
        .history_filename = "examples/ALNS.csv",
        .max_threads = 1,
        .max_iterations = 10'000,
        .max_iterations_without_improvement = 2500};

    // Overwrite some parameters from commandline. 
    parser.option<size_t>(params.max_iterations, "iterations", "ALNS maximum number of iterations");
    parser.option<std::filesystem::path>(params.history_filename, "outputlog", "ALNS history filename (.csv)");
    parser.option<size_t>(params.max_threads, "j", "ALNS maximum threads");
    parser.parse_or_exit(argc, argv);

    DemoSolution demo(40.0f, 10.0f);

    std::vector<Nilib::ALNS::Solver<DemoSolution>::Operator> operators = {op1, op2};

    Nilib::ALNS::Solver<DemoSolution> solver(params, demo, operators);

    solver.setGlobalImprovementCallBack(myfunction);
    // solver.setIterationCallback();

    auto history = solver.solve();

    LOG_INFO("Best found solution ({},{}) objective: {} iterations: {}", solver.bestfound.x, solver.bestfound.y, solver.bestfound.objective(), history.objectives.size());
    LOG_SUCCESS("Closing Demo.exe!");
    return 1;
}