#include "Nilib/Optimization/MixedIntegerLinearProgram.hpp"
#include "Nilib/Logger/Log.hpp"
#include <iostream>

using namespace Nilib::MixedIntegerLinearProgram;

void create_milp(Options options);

int main()
{
    // Construct a simple mixed integer linear program
    Options default_options;

    Options opt1;
    opt1.backend = Options::Backend::GUROBI;
    opt1.verbosity = Options::Verbosity::QUIET;

    Options opt2;
    opt2.backend = Options::Backend::HIGHS;
    opt2.verbosity = Options::Verbosity::QUIET;

    // create_milp(default_options);
    create_milp(opt1);
    create_milp(opt2);

    LOG_SUCCESS("Succesfully ran MixedIntegerLinearProgram.exe!");
    return EXIT_SUCCESS;
}

void create_milp(Options options)
{
    LOG_INFO("Creating a MILP!");

    // Set some options.
    options.sense = Options::Sense::MAXIMIZE;

    // Create the program. 
    MixedIntegerLinearProgram milp(options);

    // Add some variables.
    uint32_t x = milp.addVar(5.0, VarType::CONTINUOUS, 0.0, 20.0);
    uint32_t y = milp.addVar(2.0, VarType::CONTINUOUS, 0.0, 20.0);

    // Add some constraints.
    uint32_t c = milp.addConstraint({x, y}, {1.0, 1.0}, 5.0, ConstrType::LEQ);

    // Solve the model.
    milp.solve();

    // Print the solution.
    if (milp.status() == Status::OPTIMAL)
    {
        LOG_WARNING("Solved! obj:", milp.objective(), "x:", milp.variable_value(x), 'y', milp.variable_value(y), "dual:", milp.dual(c));
    }
    // Add another constraint.
    uint32_t c1 = milp.addConstraint({x, y}, {2.0, 2.0}, 5.0, ConstrType::LEQ);
    
    // Solve the model.
    milp.solve();

    // Print the solution.
    if (milp.status() == Status::OPTIMAL)
    {
        LOG_WARNING("Solved! obj:", milp.objective(), "x:", milp.variable_value(x), 'y', milp.variable_value(y), "dual:", milp.dual(c));
    }
}
