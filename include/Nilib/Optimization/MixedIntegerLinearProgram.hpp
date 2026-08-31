#ifndef _LINEAR_PROGRAM_HPP
#define _LINEAR_PROGRAM_HPP

#include <memory>
#include <vector>

namespace Nilib
{

    namespace MixedIntegerLinearProgram
    {

        enum class Status
        {
            OPTIMAL,
            INFEASIBLE,
            UNBOUNDED,
            UNKNOWN
        };
        enum class VarType
        {
            CONTINUOUS,
            BINARY,
            INTEGER
        };
        enum class ConstrType
        {
            EQ,  // ==
            LEQ, // <=
            GEQ  // >=
        };

        struct Options
        {
            // Output level
            enum class Verbosity
            {
                QUIET,
                ALL
            } verbosity = Verbosity::QUIET;

            // Model sense.
            enum class Sense
            {
                MINIMIZE,
                MAXIMIZE
            } sense = Sense::MINIMIZE;

            // Method DUAL/PRIMAL used.
            enum class Method
            {
                DUAL,
                PRIMAL
            } method = Method::PRIMAL;
            // Preferred backend.
            enum class Backend
            {
                GUROBI_BACKEND,
                HIGHS_BACKEND
            } backend = Backend::GUROBI_BACKEND;
        };

        struct MILPBackend
        {
            using var_t = uint32_t;
            using constr_t = uint32_t;

            virtual ~MILPBackend() = default;
            virtual var_t addVar(double coeff, VarType type, double lb, double ub) = 0;
            virtual var_t addColumn(double obj_coeff, VarType type, double lb, double ub, std::vector<constr_t> const &constr, std::vector<double> const &coeff) = 0;
            virtual constr_t addConstraint(std::vector<var_t> const &variables, std::vector<double> const &coeffs, double rhs, ConstrType type) = 0;

            virtual void removeConstraints(std::vector<constr_t> const &constraints) = 0;
            virtual void removeVariables(std::vector<var_t> const &variables) = 0;
            virtual void solve() = 0;
            virtual Status status() const = 0;
            virtual double objective() const = 0;
            virtual double variableValue(var_t variable) const = 0;
            virtual double variableObjective(var_t variable) const = 0;
            virtual double dual(constr_t constraint) const = 0;
        };

        // A (mixed integer) linear program interface.
        // Purpose: to abstract away implementation details of backends such as Gurobi, Cplex or Highs.
        // Supports a C-level interface.
        class MixedIntegerLinearProgram
        {
        public:
            using var_t = uint32_t;
            using constr_t = uint32_t;

            // Constructor selects an appropriate backend to use.
            MixedIntegerLinearProgram();
            explicit MixedIntegerLinearProgram(Options const &options);

            MixedIntegerLinearProgram(MixedIntegerLinearProgram const &) = delete;
            MixedIntegerLinearProgram &operator=(MixedIntegerLinearProgram const &) = delete;

            // Add variables to the model.
            var_t addVar(double coeff, VarType type, double lb, double ub);
            // std::vector<var_t> addVars(double coeff, VarType type, double lb, double ub);

            // Add constraints to the model.
            constr_t addConstraints(std::vector<var_t> const &variables, std::vector<double> const &coeffs, double rhs, ConstrType type);
            constr_t addConstraint(var_t variable, double coeff, double rhs, ConstrType type);
            // std::vector<constr_t> addConstraints(std::vector<var_t> const &variables, std::vector<double> const &coeffs);

            // Remove components of the model.
            void removeVariable(var_t variable);
            void removeConstraint(constr_t variable);

            // Add a complete column to the model.
            var_t addColumn(double obj_coeff, VarType type, double lb, double ub, std::vector<constr_t> const &constr, std::vector<double> const &coeff);

            // Add a complete row to the model.

            // Try to solve the model.
            void solve();

            Status status() const;

            // Get the objective value (after a solve).
            double objective() const;
            // Get the value of a variable (after a solve).
            double variableValue(var_t variable) const;
            // Get the objective coefficient of a variable (after a solve).
            double variableObjective(var_t variable) const;
            // Get the dual variable of a constraint (after a solve).
            double dual(constr_t constraint) const;

            size_t numConstraints() const { return d_numconstraints; }
            size_t numVariables() const { return d_numvariables; }

        private:
            std::unique_ptr<MILPBackend> d_backend;
            size_t d_numvariables;
            size_t d_numconstraints;
            bool d_model_continuous;
        };
    } // namespace MixedIntegerLinearProgram
} // namespace Nilib

#endif
