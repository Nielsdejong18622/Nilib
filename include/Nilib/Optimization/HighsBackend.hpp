#ifndef _HIGHS_BACKEND_HPP
#define _HIGHS_BACKEND_HPP

#include "Highs.h"
#include "Nilib/Optimization/MixedIntegerLinearProgram.hpp"

namespace Nilib
{
    namespace MixedIntegerLinearProgram
    {

        class HighsBackend : public MILPBackend
        {
        public:
            HighsBackend(Options const &options);
            ~HighsBackend() override = default;

            var_t addVar(double coeff,
                         VarType type,
                         double lb,
                         double ub) override;

            constr_t addConstraint(std::vector<var_t> const &variables,
                                   std::vector<double> const &coeffs,
                                   double rhs,
                                   ConstrType type) override;

            void solve() override;

            Status status() const override;

            double objective() const override;
            double variable_value(var_t variable) const override;
            double dual(constr_t constraint) const override;

        private:
            // Highs solver_;
            Highs d_env;
            HighsLp d_linearprogam;

            // Variable data
            std::vector<double> d_objective;
            std::vector<double> d_col_lb;
            std::vector<double> d_col_ub;
            std::vector<HighsVarType> d_integralvar;

            // Constraint data
            std::vector<double> d_row_lb;
            std::vector<double> d_row_ub;

            // Sparse matrix (row-wise first, converted later)
            std::vector<std::vector<var_t>> d_row_vars;
            std::vector<std::vector<double>> d_row_coeffs;

            // Solution
            HighsSolution d_solution;

            Status d_status = Status::UNKNOWN;
        };

    } // namespace MixedIntegerLinearProgram

} // namespace Nilib

#endif