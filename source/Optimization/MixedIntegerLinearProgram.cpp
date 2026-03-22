#include "Nilib/Optimization/MixedIntegerLinearProgram.hpp"
#include "Nilib/Optimization/GurobiBackend.hpp"
#include "Nilib/Optimization/HighsBackend.hpp"

#include "Nilib/Core/Assert.hpp"

using namespace Nilib::MixedIntegerLinearProgram;

MixedIntegerLinearProgram::MixedIntegerLinearProgram(Options const &options)
    : d_backend(nullptr), d_model_continuous(true)
{
    if (options.backend == Options::Backend::GUROBI)
    {
        d_backend = std::make_unique<GurobiBackend>(options);
    }
    else
    {
        d_backend = std::make_unique<HighsBackend>(options);
    }
    CORE_ASSERT(d_backend);
}

MixedIntegerLinearProgram::MixedIntegerLinearProgram()
    : d_backend(std::make_unique<HighsBackend>(Options{})),
      d_model_continuous(true)
{
    CORE_ASSERT(d_backend);
}

MixedIntegerLinearProgram::var_t MixedIntegerLinearProgram::addVar(double coeff, VarType type, double lb, double ub)
{
    d_model_continuous = (d_model_continuous) ? (type == VarType::CONTINUOUS) : false;
    return d_backend->addVar(coeff, type, lb, ub);
}

MixedIntegerLinearProgram::constr_t MixedIntegerLinearProgram::addConstraint(std::vector<var_t> const &variables, std::vector<double> const &coeffs, double rhs, ConstrType type)
{
    ASSERT(variables.size() == coeffs.size(), "Dimensions of coefficients and variables does not match!");
    return d_backend->addConstraint(variables, coeffs, rhs, type);
}

void MixedIntegerLinearProgram::solve()
{
    d_backend->solve();
}

Status MixedIntegerLinearProgram::status() const
{
    return d_backend->status();
}

double MixedIntegerLinearProgram::objective() const
{
    // CORE_ASSERT(model.status == SOLVED);
    return d_backend->objective();
}

double MixedIntegerLinearProgram::variable_value(var_t variable) const
{
    // CORE_ASSERT(model.status == SOLVED);
    return d_backend->variable_value(variable);
}

double MixedIntegerLinearProgram::dual(constr_t constraint) const
{
    ASSERT(d_model_continuous, "Model contains integral variable, dual not available!");
    return d_backend->dual(constraint);
}
