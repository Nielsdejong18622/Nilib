#include "Nilib/Math/ALNS.hpp"

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Logger/CSV.hpp"

#include "Nilib/Math/RNG.hpp"
#include "Nilib/Core/ArgParser.hpp"

struct DemoSolution : public Nilib::ALNS::Solution
{
    float x, y;

    DemoSolution(float x, float y)
        : x(x), y(y) {}

    float objective() const
    {
        // Simple convex problem.
        return (x - 5.0f) * (x - 5.0f) + (y - 15.0f) * (y - 15.0f);

        // Rastrigin function.
        // return 20 + x * x + y * y - 10 * (cos(6.28 * x) + cos(6.28 * y));

        // // Shaffer F6.
        // float const parabola = x * x + y * y;
        // float const denom = (1 + 0.001 * parabola);
        // return 100 * (0.5 + (sin(sqrt(parabola)) * sin(sqrt(parabola)) - 0.5) / (denom * denom));
    }

    bool feasible() const 
    {
        // Always feasible. 
        return true;
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
    LOG_DEBUG() << "Found global improvement:" << sol.objective() << '\n';
}

int main(int argc, char **argv)
{

    Nilib::Argparser parser(argc, argv);
    Nilib::RNG::seed();
    
    Nilib::ALNS::ALNSparams params;

    DemoSolution demo(40.0f, 10.0f);

    std::vector<Nilib::ALNS::Solver<DemoSolution>::Operator> operators = {op1, op2};

    params.history_filename = parser.argument<std::string>("--outputfile", "-o", "ALNS history filename (.csv)", "ALNS.csv");
    params.max_iterations = parser.argument<size_t>("--iterations", "-i", "ALNS maximum number of iterations", 1000);
    parser.check();

    Nilib::ALNS::Solver<DemoSolution> solver(params, operators, demo);

    solver.setGlobalImprovementCallBack(myfunction);
    // solver.setIterationCallback();

    solver.solve();

    LOG_INFO("Best found solution", solver.bestfound.x, solver.bestfound.y, "objective:", solver.bestfound.objective());
    LOG_SUCCESS("Closing Demo.exe!");
    return 1;
}