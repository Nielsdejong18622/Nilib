#include "Nilib/Optimization/MixedIntegerLinearProgram.hpp"
#include "Nilib/Optimization/GurobiBackend.hpp"
#include "Nilib/Optimization/HighsBackend.hpp"

#include "Nilib/Core/Assert.hpp"

using namespace Nilib::MixedIntegerLinearProgram;

MixedIntegerLinearProgram::MixedIntegerLinearProgram(Options const &options)
    : d_backend(nullptr), d_numvariables(0), d_numconstraints(0), d_model_continuous(true)
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
    d_numvariables++;
    d_model_continuous = (d_model_continuous) ? (type == VarType::CONTINUOUS) : false;
    return d_backend->addVar(coeff, type, lb, ub);
}

MixedIntegerLinearProgram::constr_t MixedIntegerLinearProgram::addConstraint(var_t variable, double coeff, double rhs, ConstrType type)
{
    d_numconstraints++;
    return d_backend->addConstraint({variable}, {coeff}, rhs, type);
}

void Nilib::MixedIntegerLinearProgram::MixedIntegerLinearProgram::removeVariable(var_t variable)
{
    d_numvariables--;
    d_backend->removeVariables({variable});
}
void Nilib::MixedIntegerLinearProgram::MixedIntegerLinearProgram::removeConstraint(constr_t constraint)
{
    d_numconstraints--;
    d_backend->removeConstraints({constraint});
}

Nilib::MixedIntegerLinearProgram::MixedIntegerLinearProgram::var_t Nilib::MixedIntegerLinearProgram::MixedIntegerLinearProgram::addColumn(double obj_coeff, VarType type, double lb, double ub, std::vector<constr_t> const &constr, std::vector<double> const &coeff)
{
    return d_backend->addColumn(obj_coeff, type, lb, ub, constr, coeff);
}

MixedIntegerLinearProgram::constr_t MixedIntegerLinearProgram::addConstraints(std::vector<var_t> const &variables, std::vector<double> const &coeffs, double rhs, ConstrType type)
{
    d_numvariables += variables.size();
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
    CORE_ASSERT(status() == Status::OPTIMAL);
    return d_backend->objective();
}

double MixedIntegerLinearProgram::variableObjective(var_t variable) const
{
    return d_backend->variableObjective(variable);
}

double MixedIntegerLinearProgram::variableValue(var_t variable) const
{
    CORE_ASSERT(status() == Status::OPTIMAL);
    return d_backend->variableValue(variable);
}

double MixedIntegerLinearProgram::dual(constr_t constraint) const
{
    CORE_ASSERT(status() == Status::OPTIMAL);
    ASSERT(d_model_continuous, "Model contains integral variable, dual not available!");
    return d_backend->dual(constraint);
}
