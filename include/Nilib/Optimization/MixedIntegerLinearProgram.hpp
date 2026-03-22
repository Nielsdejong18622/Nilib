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
                GUROBI,
                HIGHS
            } backend = Backend::GUROBI;
        };

        struct MILPBackend
        {
            using var_t = uint32_t;
            using constr_t = uint32_t;

            virtual ~MILPBackend() = default;
            virtual var_t addVar(double coeff, VarType type, double lb, double ub) = 0;
            virtual constr_t addConstraint(std::vector<var_t> const &variables, std::vector<double> const &coeffs, double rhs, ConstrType type) = 0;
            virtual void solve() = 0;
            virtual Status status() const = 0;
            virtual double objective() const = 0;
            virtual double variable_value(var_t variable) const = 0;
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
            constr_t addConstraint(std::vector<var_t> const &variables, std::vector<double> const &coeffs, double rhs, ConstrType type);
            // std::vector<constr_t> addConstraints(std::vector<var_t> const &variables, std::vector<double> const &coeffs);

            // Add a complete column to the model.

            // Add a complete row to the model.

            // Try to solve the model.
            void solve();

            Status status() const;

            // Get the objective value (after a solve).
            double objective() const;
            // Get the value of a variable (after a solve).
            double variable_value(var_t variable) const;
            // Get the dual variable of a constraint (after a solve).
            double dual(constr_t constraint) const;

        private:
            std::unique_ptr<MILPBackend> d_backend;
            bool d_model_continuous;
        };
    } // namespace MixedIntegerLinearProgram
} // namespace Nilib

#endif
