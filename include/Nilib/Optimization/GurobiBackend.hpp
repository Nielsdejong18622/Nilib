#ifndef _GUROBI_BACKEND_HPP
#define _GUROBI_BACKEND_HPP

#include "gurobi_c++.h"
#include "Nilib/Optimization/MixedIntegerLinearProgram.hpp"

namespace Nilib
{
    namespace MixedIntegerLinearProgram
    {

        class GurobiBackend : public MILPBackend
        {
        public:
            GurobiBackend(Options const &options);
            ~GurobiBackend() override = default;

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
        
            std::unique_ptr<GRBEnv> d_env;
            std::unique_ptr<GRBModel> d_model;

            std::vector<GRBVar> d_vars;
            std::vector<GRBConstr> d_constrs;

            Status d_status = Status::UNKNOWN;
        };

    } // namespace MixedIntegerLinearProgram

} // namespace Nilib

#endif