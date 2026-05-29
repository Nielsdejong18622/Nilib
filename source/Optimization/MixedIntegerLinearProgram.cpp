#include "Nilib/Optimization/MixedIntegerLinearProgram.hpp"
#include "Nilib/Optimization/GurobiBackend.hpp"
#include "Nilib/Optimization/HighsBackend.hpp"

#include "Nilib/Core/Assert.hpp"

using namespace Nilib::MixedIntegerLinearProgram;

MixedIntegerLinearProgram::MixedIntegerLinearProgram(Options const &options)
    : d_backend(nullptr), d_model_continuous(true)
{
    if (options.backend == Options::Backend::GUROBI_BACKEND)
    {
#ifdef GUROBI
        d_backend = std::make_unique<GurobiBackend>(options);
#else
        ASSERT(false, "Gurobi backend not enabled during compilation!");
#endif
    }
    else
    {
#ifdef HIGHS
        d_backend = std::make_unique<HighsBackend>(options);
#else
        ASSERT(false, "HiGHS backend not enabled during compilation!");
#endif
    }
    CORE_ASSERT(d_backend);
}

MixedIntegerLinearProgram::MixedIntegerLinearProgram()
    : MixedIntegerLinearProgram(Options{.backend = Options::Backend::HIGHS_BACKEND})
{
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
