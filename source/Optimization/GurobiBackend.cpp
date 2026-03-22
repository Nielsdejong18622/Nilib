#include "Nilib/Optimization/GurobiBackend.hpp"

using namespace Nilib::MixedIntegerLinearProgram;

GurobiBackend::GurobiBackend(Options const &options)
{
    d_env = std::make_unique<GRBEnv>(true);
    d_env->set(GRB_IntParam_OutputFlag, (options.verbosity == Options::Verbosity::QUIET) ? 0 : 1);
    d_env->set(GRB_IntParam_LogToConsole, (options.verbosity == Options::Verbosity::QUIET) ? 0 : 1);
    d_env->start();

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
    return static_cast<var_t>(d_vars.size() - 1);
}

MILPBackend::constr_t GurobiBackend::addConstraint(std::vector<var_t> const &variables,
                                                   std::vector<double> const &coeffs,
                                                   double rhs,
                                                   ConstrType type)
{
    if (variables.size() != coeffs.size())
        throw std::runtime_error("Mismatched constraint sizes");

    GRBLinExpr expr = 0.0;

    for (size_t i = 0; i < variables.size(); ++i)
    {
        expr += coeffs[i] * d_vars[variables[i]];
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

    d_model->write("model_GRB.mps");
}

Status GurobiBackend::status() const
{
    return d_status;
}

double GurobiBackend::objective() const
{
    return d_model->get(GRB_DoubleAttr_ObjVal);
}

double GurobiBackend::variable_value(var_t variable) const
{
    return d_vars.at(variable).get(GRB_DoubleAttr_X);
}

double GurobiBackend::dual(constr_t constraint) const
{
    return d_constrs.at(constraint).get(GRB_DoubleAttr_Pi);
}