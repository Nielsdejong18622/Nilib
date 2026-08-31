#include "Nilib/Optimization/HighsBackend.hpp"
#include "Nilib/Core/Assert.hpp"

#ifdef HIGHS

using namespace Nilib::MixedIntegerLinearProgram;

HighsBackend::HighsBackend(Options const &options)
{
    d_env.setOptionValue(
        "output_flag",
        options.verbosity != Options::Verbosity::QUIET);

    if (options.sense == Options::Sense::MAXIMIZE)
        d_linearprogam.sense_ = ObjSense::kMaximize;
    else
        d_linearprogam.sense_ = ObjSense::kMinimize;
}

MILPBackend::var_t HighsBackend::addVar(
    double coeff,
    VarType type,
    double lb,
    double ub)
{
    var_t variable = static_cast<var_t>(d_objective.size());

    d_objective.push_back(coeff);
    d_col_lb.push_back(lb);
    d_col_ub.push_back(ub);
    d_active_vars.push_back(true);

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

    return variable;
}

MILPBackend::constr_t HighsBackend::addConstraint(
    std::vector<var_t> const &variables,
    std::vector<double> const &coeffs,
    double rhs,
    ConstrType type)
{
    ASSERT(variables.size() == coeffs.size(), "Constraint size mismatch");

    for (var_t variable : variables)
    {
        CORE_ASSERT(variable >= 0);
        CORE_ASSERT(variable < static_cast<var_t>(d_objective.size()));
        CORE_ASSERT(d_active_vars[variable]);
    }

    constr_t constraint = static_cast<constr_t>(d_row_vars.size());

    d_row_vars.push_back(variables);
    d_row_coeffs.push_back(coeffs);
    d_active_constraints.push_back(true);

    double const inf = kHighsInf;

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

    return constraint;
}

HighsBackend::var_t HighsBackend::addColumn(
    double obj_coeff,
    VarType type,
    double lb,
    double ub,
    std::vector<constr_t> const &constr,
    std::vector<double> const &coeff)
{
    CORE_ASSERT(constr.size() == coeff.size());

    var_t variable = addVar(obj_coeff, type, lb, ub);

    for (size_t i = 0; i < constr.size(); ++i)
    {
        constr_t constraint = constr[i];

        CORE_ASSERT(constraint >= 0);
        CORE_ASSERT(constraint < static_cast<constr_t>(d_row_vars.size()));
        CORE_ASSERT(d_active_constraints[constraint]);

        d_row_vars[constraint].push_back(variable);
        d_row_coeffs[constraint].push_back(coeff[i]);
    }

    return variable;
}

void HighsBackend::removeConstraints(std::vector<constr_t> const &constraints)
{
    for (constr_t constraint : constraints)
    {
        CORE_ASSERT(constraint >= 0);
        CORE_ASSERT(constraint < static_cast<constr_t>(d_row_vars.size()));
        CORE_ASSERT(d_active_constraints[constraint]);

        d_active_constraints[constraint] = false;
    }
}

void HighsBackend::removeVariables(std::vector<var_t> const &variables)
{
    for (var_t variable : variables)
    {
        CORE_ASSERT(variable >= 0);
        CORE_ASSERT(variable < static_cast<var_t>(d_objective.size()));
        CORE_ASSERT(d_active_vars[variable]);

        d_active_vars[variable] = false;
    }

    // Remove references to deleted variables from constraints.
    // The variable IDs themselves are NOT changed.
    for (size_t row = 0; row < d_row_vars.size(); ++row)
    {
        if (!d_active_constraints[row])
            continue;

        auto &vars = d_row_vars[row];
        auto &coeffs = d_row_coeffs[row];

        CORE_ASSERT(vars.size() == coeffs.size());

        size_t write = 0;

        for (size_t read = 0; read < vars.size(); ++read)
        {
            var_t variable = vars[read];

            if (!d_active_vars[variable])
                continue;

            vars[write] = vars[read];
            coeffs[write] = coeffs[read];

            ++write;
        }

        vars.resize(write);
        coeffs.resize(write);
    }
}

void HighsBackend::solve()
{
    // ------------------------------------------------------------
    // Build stable variable -> HiGHS column mapping.
    // ------------------------------------------------------------

    int num_col = 0;

    d_var_to_col.assign(d_objective.size(), -1);

    for (var_t variable = 0;
         variable < static_cast<var_t>(d_objective.size());
         ++variable)
    {
        if (!d_active_vars[variable])
            continue;

        d_var_to_col[variable] = num_col++;
    }

    // ------------------------------------------------------------
    // Build stable constraint -> HiGHS row mapping.
    // ------------------------------------------------------------

    int num_row = 0;

    d_constr_to_row.assign(
        d_row_vars.size(),
        -1);

    for (constr_t constraint = 0;
         constraint < static_cast<constr_t>(d_row_vars.size());
         ++constraint)
    {
        if (!d_active_constraints[constraint])
            continue;

        d_constr_to_row[constraint] = num_row++;
    }

    d_linearprogam.num_col_ = num_col;
    d_linearprogam.num_row_ = num_row;

    // ------------------------------------------------------------
    // Build compact variable arrays.
    // ------------------------------------------------------------

    d_linearprogam.col_cost_.resize(num_col);
    d_linearprogam.col_lower_.resize(num_col);
    d_linearprogam.col_upper_.resize(num_col);
    d_linearprogam.integrality_.resize(num_col);

    for (var_t variable = 0; variable < static_cast<var_t>(d_objective.size()); ++variable)
    {
        if (!d_active_vars[variable])
            continue;

        int col = d_var_to_col[variable];

        d_linearprogam.col_cost_[col] = d_objective[variable];
        d_linearprogam.col_lower_[col] = d_col_lb[variable];
        d_linearprogam.col_upper_[col] = d_col_ub[variable];
        d_linearprogam.integrality_[col] = d_integralvar[variable];
    }

    // ------------------------------------------------------------
    // Build compact constraint arrays.
    // ------------------------------------------------------------

    d_linearprogam.row_lower_.resize(num_row);
    d_linearprogam.row_upper_.resize(num_row);

    for (constr_t constraint = 0; constraint < static_cast<constr_t>(d_row_vars.size()); ++constraint)
    {
        if (!d_active_constraints[constraint])
            continue;

        int row = d_constr_to_row[constraint];

        d_linearprogam.row_lower_[row] = d_row_lb[constraint];
        d_linearprogam.row_upper_[row] = d_row_ub[constraint];
    }

    // ------------------------------------------------------------
    // Build column-wise sparse matrix.
    // ------------------------------------------------------------

    std::vector<std::vector<std::pair<int, double>>> col_entries(
        num_col);

    for (constr_t constraint = 0; constraint < static_cast<constr_t>(d_row_vars.size()); ++constraint)
    {
        if (!d_active_constraints[constraint])
            continue;

        int row = d_constr_to_row[constraint];

        CORE_ASSERT(d_row_vars[constraint].size() == d_row_coeffs[constraint].size());

        for (size_t k = 0; k < d_row_vars[constraint].size(); ++k)
        {
            var_t variable = d_row_vars[constraint][k];

            CORE_ASSERT(variable >= 0);
            CORE_ASSERT(variable < static_cast<var_t>(d_objective.size()));

            // This should normally not occur because
            // removeVariables() cleans up the constraints.
            if (!d_active_vars[variable])
                continue;

            int col = d_var_to_col[variable];

            CORE_ASSERT(col >= 0);
            CORE_ASSERT(col < num_col);

            double coefficient = d_row_coeffs[constraint][k];

            col_entries[col].emplace_back(row, coefficient);
        }
    }

    // ------------------------------------------------------------
    // Convert to compressed sparse column format.
    // ------------------------------------------------------------

    std::vector<int> start(num_col + 1, 0);
    std::vector<int> index;
    std::vector<double> value;

    for (int col = 0; col < num_col; ++col)
    {
        start[col + 1] = start[col] + static_cast<int>(col_entries[col].size());
    }

    index.resize(start.back());
    value.resize(start.back());

    for (int col = 0; col < num_col; ++col)
    {
        int pos = start[col];

        for (auto const &entry : col_entries[col])
        {
            index[pos] = entry.first;
            value[pos] = entry.second;
            ++pos;
        }
    }

    d_linearprogam.a_matrix_.start_ = std::move(start);
    d_linearprogam.a_matrix_.index_ = std::move(index);
    d_linearprogam.a_matrix_.value_ = std::move(value);

    // ------------------------------------------------------------
    // Solve.
    // ------------------------------------------------------------

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

double HighsBackend::variableValue(var_t variable) const
{
    CORE_ASSERT(variable >= 0);
    CORE_ASSERT(variable < static_cast<var_t>(d_active_vars.size()));
    CORE_ASSERT(d_active_vars[variable]);

    int col = d_var_to_col[variable];

    CORE_ASSERT(col >= 0);

    return d_solution.col_value[col];
}

double HighsBackend::variableObjective(var_t variable) const
{
    CORE_ASSERT(variable >= 0);
    CORE_ASSERT(variable < static_cast<var_t>(d_objective.size()));
    CORE_ASSERT(d_active_vars[variable]);

    return d_objective[variable];
}

double HighsBackend::dual(constr_t constraint) const
{
    CORE_ASSERT(constraint >= 0);
    CORE_ASSERT(constraint < static_cast<constr_t>(d_active_constraints.size()));

    CORE_ASSERT(d_active_constraints[constraint]);
    CORE_ASSERT(d_solution.dual_valid);

    int row = d_constr_to_row[constraint];

    CORE_ASSERT(row >= 0);

    return d_solution.row_dual[row];
}

#endif // HIGHS