#ifndef _GUROBI_BACKEND_HPP
#define _GUROBI_BACKEND_HPP

#include "Nilib/Optimization/MixedIntegerLinearProgram.hpp"
#ifdef GUROBI
#include "gurobi_c++.h"

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

            var_t addColumn(double obj_coeff, VarType type, double lb, double ub, std::vector<constr_t> const &constr, std::vector<double> const &coeff) override;
            void removeConstraints(std::vector<constr_t> const &constraints) override;
            void removeVariables(std::vector<var_t> const &variables) override;
            void solve() override;

            Status status() const override;

            double objective() const override;
            double variableValue(var_t variable) const override;
            double variableObjective(var_t variable) const override;
            double dual(constr_t constraint) const override;

        private:
            std::unique_ptr<GRBEnv> d_env;
            std::unique_ptr<GRBModel> d_model;
            
            std::vector<GRBVar> d_vars;
            std::vector<GRBConstr> d_constrs;

            std::vector<bool> d_active_vars;
            std::vector<bool> d_active_constrs;

            Status d_status = Status::UNKNOWN;
        };

    } // namespace MixedIntegerLinearProgram

} // namespace Nilib

#endif // GUROBI
#endif