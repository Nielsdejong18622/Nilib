#ifndef _VRP_SOLUTION_H
#define _VRP_SOLUTION_H

#include "Nilib/Structures/VRPInstance.hpp"
#include "Nilib/Core/Serializer.hpp"
#include "Nilib/Core/Deserializer.hpp"

namespace Nilib
{
    namespace VRP
    {
        class Solution
        {
        private:
        public:
            // A solution object is meaningless without an instance.
            Instance d_instance;

            // The route matrix.
            Nilib::Matrixf d_X;
            float d_opt_objval = 0.0;

            Solution(Instance const &inst, Nilib::Matrixf const &X, float const opt_objval);
            Solution(Instance const &inst, Nilib::Matrixf const &X);

            // Shuffle the solution.
            void random();

            static Solution randomSolution(Instance const &inst);
            static Solution optimalSolution(Instance const &inst);
            static Solution feasibleSolution(Instance const &inst);

            void draw(Nilib::Window const &window) const;

            // Returns information about the solution.
            bool feasible() const;
            float objective() const;
            float optimalityGap() const;

            bool serialize(Serializer &serializer);
            bool deserialize(Deserializer &deserializer);

            bool operator==(Solution const &other) const;
        };
    };
};

#endif