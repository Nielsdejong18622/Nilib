#include "Nilib/Optimization/HighsBackend.hpp"
#include "Nilib/Core/Assert.hpp"

// #include <stdexcept>
// #include <limits>

using namespace Nilib::MixedIntegerLinearProgram;

HighsBackend::HighsBackend(Options const &options)
{
    // if (options.silent)
    //     highs_.setOptionValue("output_flag", false);

    // if (options.time_limit > 0.0)
    //     highs_.setOptionValue("time_limit", options.time_limit);

    // if (options.num_threads > 0)
    //     highs_.setOptionValue("threads", options.num_threads);
    d_env.setOptionValue("output_flag", (options.verbosity == Options::Verbosity::ALL));
    if (options.sense == Options::Sense::MAXIMIZE)
        d_linearprogam.sense_ = ObjSense::kMaximize;
    else
        d_linearprogam.sense_ = ObjSense::kMinimize;
}

MILPBackend::var_t HighsBackend::addVar(double coeff, VarType type, double lb, double ub)
{
    d_objective.push_back(coeff);
    d_col_lb.push_back(lb);
    d_col_ub.push_back(ub);

    switch (type)
    {
    case VarType::INTEGER:
        d_integralvar.push_back(HighsVarType::kInteger);
        break;
    case VarType::BINARY:
        d_integralvar.push_back(HighsVarType::kInteger);
        break;
    default:
        d_integralvar.push_back(HighsVarType::kContinuous);
        break;
    }
    return static_cast<var_t>(d_objective.size() - 1);
}

MILPBackend::constr_t HighsBackend::addConstraint(std::vector<var_t> const &variables,
                                                  std::vector<double> const &coeffs,
                                                  double rhs,
                                                  ConstrType type)
{
    ASSERT(variables.size() == coeffs.size(), "Constraint size mismatch");

    d_row_vars.push_back(variables);
    d_row_coeffs.push_back(coeffs);

    double inf = kHighsInf;

    switch (type)
    {
    case ConstrType::LEQ:
        d_row_lb.push_back(-inf);
        d_row_ub.push_back(rhs);
        break;
    case ConstrType::GEQ:
        d_row_lb.push_back(rhs);
        d_row_ub.push_back(inf);
        break;
    default:
        d_row_lb.push_back(rhs);
        d_row_ub.push_back(rhs);
        break;
    }

    return static_cast<constr_t>(d_row_vars.size() - 1);
}

void HighsBackend::solve()
{

    int const num_col = d_objective.size();
    int const num_row = d_row_lb.size();

    // d_linearprogam.sense_ = ; // or kMaximize

    d_linearprogam.num_col_ = num_col;
    d_linearprogam.num_row_ = num_row;

    d_linearprogam.col_cost_ = d_objective;
    d_linearprogam.col_lower_ = d_col_lb;
    d_linearprogam.col_upper_ = d_col_ub;

    d_linearprogam.row_lower_ = d_row_lb;
    d_linearprogam.row_upper_ = d_row_ub;

    d_linearprogam.integrality_ = d_integralvar;

    // Build column-wise sparse matrix
    std::vector<std::vector<std::pair<int, double>>> col_entries(num_col);

    for (int r = 0; r < num_row; ++r)
    {
        for (size_t k = 0; k < d_row_vars[r].size(); ++k)
        {
            int c = d_row_vars[r][k];
            assert(c >= 0 && c < num_col); // Safety check
            double coeff = d_row_coeffs[r][k];
            col_entries[c].emplace_back(r, coeff);
        }
    }

    std::vector<int> start(num_col + 1, 0);
    std::vector<int> index;
    std::vector<double> value;

    // Compute prefix sum for start
    for (int c = 0; c < num_col; ++c)
    {
        start[c + 1] = start[c] + static_cast<int>(col_entries[c].size());
    }

    index.resize(start.back());
    value.resize(start.back());

    // Fill index and value
    for (int c = 0; c < num_col; ++c)
    {
        int pos = start[c];
        for (auto &entry : col_entries[c])
        {
            index[pos] = entry.first;  // row index
            value[pos] = entry.second; // coefficient
            ++pos;
        }
    }

    d_linearprogam.a_matrix_.start_ = std::move(start);
    d_linearprogam.a_matrix_.index_ = std::move(index);
    d_linearprogam.a_matrix_.value_ = std::move(value);
    // Solve the model.
    d_env.passModel(d_linearprogam);
    d_env.run();

    d_solution = d_env.getSolution();

    HighsModelStatus model_status = d_env.getModelStatus();

    switch (model_status)
    {
    case HighsModelStatus::kOptimal:
        d_status = Status::OPTIMAL;
        break;
    case HighsModelStatus::kInfeasible:
        d_status = Status::INFEASIBLE;
        break;
    case HighsModelStatus::kUnbounded:
        d_status = Status::UNBOUNDED;
        break;
    default:
        d_status = Status::UNKNOWN;
        break;
    }
}

Status HighsBackend::status() const
{
    return d_status;
}

double HighsBackend::objective() const
{
    return d_env.getObjectiveValue();
}

double HighsBackend::variable_value(var_t variable) const
{
    return d_solution.col_value.at(variable);
}

double HighsBackend::dual(constr_t constraint) const
{
    CORE_ASSERT(d_solution.dual_valid);
    return d_solution.row_dual.at(constraint);
}