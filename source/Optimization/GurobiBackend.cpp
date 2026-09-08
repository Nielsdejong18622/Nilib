#include "Nilib/Optimization/GurobiBackend.hpp"
#include "Nilib/Core/Assert.hpp"

using namespace Nilib::MixedIntegerLinearProgram;

#ifdef GUROBI

GurobiBackend::GurobiBackend(Options const &options)
{
    d_env = std::make_unique<GRBEnv>(true);

    d_env->set(GRB_IntParam_OutputFlag, (options.verbosity == Options::Verbosity::QUIET) ? 0 : 1);
    d_env->set(GRB_IntParam_LogToConsole, (options.verbosity == Options::Verbosity::QUIET) ? 0 : 1);
    try
    {
        d_env->start();
    }
    catch (GRBException const &e)
    {
        LOG_ERROR("Failed to start Gurobi environment. Error:", e.getErrorCode(), e.getMessage());
        throw e;
    }

    d_model = std::make_unique<GRBModel>(*d_env);
    // Options.
    d_model->set(GRB_IntAttr_ModelSense, (options.sense == Options::Sense::MINIMIZE) ? GRB_MINIMIZE : GRB_MAXIMIZE);
    d_model->set(GRB_IntParam_OutputFlag, (options.verbosity == Options::Verbosity::QUIET) ? 0 : 1);
    d_model->set(GRB_IntParam_LogToConsole, (options.verbosity == Options::Verbosity::QUIET) ? 0 : 1);
}

MILPBackend::var_t GurobiBackend::addVar(double coeff, VarType type, double lb, double ub)
{
    char vtype;
    switch (type)
    {
    case VarType::INTEGER:
        vtype = GRB_INTEGER;
        break;
    case VarType::BINARY:
        vtype = GRB_BINARY;
        break;
    default:
        vtype = GRB_CONTINUOUS;
        break;
    }
    GRBVar var = d_model->addVar(lb, ub, coeff, vtype);
    d_vars.push_back(var);
    d_active_vars.push_back(true);
    return static_cast<var_t>(d_vars.size() - 1);
}

GurobiBackend::var_t GurobiBackend::addColumn(double obj_coeff, VarType type, double lb, double ub, std::vector<constr_t> const &constrs, std::vector<double> const &coeffs)
{
    CORE_ASSERT(constrs.size() == coeffs.size());
    GRBColumn col;
    for (size_t i = 0; i < constrs.size(); ++i)
    {
        constr_t constraint = constrs[i];
        CORE_ASSERT(constraint >= 0);
        CORE_ASSERT(constraint < static_cast<constr_t>(d_constrs.size()));
        CORE_ASSERT(d_active_constrs[constraint]);
        col.addTerm(coeffs[i], d_constrs[constraint]);
    }
    char vtype;
    switch (type)
    {
    case VarType::INTEGER:
        vtype = GRB_INTEGER;
        break;
    case VarType::BINARY:
        vtype = GRB_BINARY;
        break;
    default:
        vtype = GRB_CONTINUOUS;
        break;
    }
    GRBVar var = d_model->addVar(lb, ub, obj_coeff, vtype, col);
    d_vars.push_back(var);
    d_active_vars.push_back(true);
    return static_cast<var_t>(d_vars.size() - 1);
}
MILPBackend::constr_t GurobiBackend::addConstraint(
    std::vector<var_t> const &variables,
    std::vector<double> const &coeffs,
    double rhs,
    ConstrType type)
{
    CORE_ASSERT(variables.size() == coeffs.size());

    GRBLinExpr expr = 0.0;

    for (size_t i = 0; i < variables.size(); ++i)
    {
        var_t variable = variables[i];

        CORE_ASSERT(variable >= 0);
        CORE_ASSERT(variable < static_cast<var_t>(d_vars.size()));
        CORE_ASSERT(d_active_vars[variable]);

        expr += coeffs[i] * d_vars[variable];
    }

    GRBConstr c;

    switch (type)
    {
    case ConstrType::GEQ:
        c = d_model->addConstr(expr >= rhs);
        break;

    case ConstrType::LEQ:
        c = d_model->addConstr(expr <= rhs);
        break;

    default:
        c = d_model->addConstr(expr == rhs);
        break;
    }

    d_constrs.push_back(c);
    d_active_constrs.push_back(true); // <-- missing

    return static_cast<constr_t>(d_constrs.size() - 1);
}

void GurobiBackend::solve()
{
    d_model->optimize();

    int grb_status = d_model->get(GRB_IntAttr_Status);

    switch (grb_status)
    {
    case GRB_OPTIMAL:
        d_status = Status::OPTIMAL;
        break;
    case GRB_INFEASIBLE:
        d_status = Status::INFEASIBLE;
        break;
    case GRB_UNBOUNDED:
        d_status = Status::UNBOUNDED;
        break;
    default:
        d_status = Status::UNKNOWN;
        break;
    }
}

Status GurobiBackend::status() const
{
    return d_status;
}

double GurobiBackend::objective() const
{
    return d_model->get(GRB_DoubleAttr_ObjVal);
}

double GurobiBackend::variableValue(var_t variable) const
{
    CORE_ASSERT(variable >= 0);
    CORE_ASSERT(variable < static_cast<var_t>(d_vars.size()));
    CORE_ASSERT(d_active_vars[variable]);
    return d_vars.at(variable).get(GRB_DoubleAttr_X);
}
double GurobiBackend::variableObjective(var_t variable) const
{
    CORE_ASSERT(variable >= 0);
    CORE_ASSERT(variable < static_cast<var_t>(d_vars.size()));
    CORE_ASSERT(d_active_vars[variable]);
    return d_vars.at(variable).get(GRB_DoubleAttr_Obj);
}
double GurobiBackend::dual(constr_t constraint) const
{
    CORE_ASSERT(constraint >= 0);
    CORE_ASSERT(constraint < static_cast<constr_t>(d_constrs.size()));
    CORE_ASSERT(d_active_constrs[constraint]);
    return d_constrs.at(constraint).get(GRB_DoubleAttr_Pi);
}

void GurobiBackend::removeConstraints(std::vector<constr_t> const &constraints)
{
    if (constraints.empty())
        return;
    for (constr_t constraint : constraints)
    {
        CORE_ASSERT(constraint >= 0);
        CORE_ASSERT(constraint < static_cast<constr_t>(d_constrs.size()));
        CORE_ASSERT(d_active_constrs[constraint]);
        d_model->remove(d_constrs[constraint]);
        d_active_constrs[constraint] = false;
    }
    d_model->update();
}
void GurobiBackend::removeVariables(std::vector<var_t> const &variables)
{
    if (variables.empty())
        return;
    for (var_t variable : variables)
    {
        CORE_ASSERT(variable >= 0);
        CORE_ASSERT(variable < static_cast<var_t>(d_vars.size()));
        CORE_ASSERT(d_active_vars[variable]);
        d_model->remove(d_vars[variable]);
        d_active_vars[variable] = false;
    }
    d_model->update();
}

#endif