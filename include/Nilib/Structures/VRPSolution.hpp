#ifndef _VRP_SOLUTION_H
#define _VRP_SOLUTION_H

#include "Nilib/Structures/VRPInstance.hpp"
#include "Nilib/Math/ALNS.hpp"

#include "Nilib/Core/Serializer.hpp"
#include "Nilib/Core/Deserializer.hpp"

namespace Nilib
{
    namespace VRP
    {     
        // Pretty basic MATRIX VRP solution. 
        struct Solution
        {
            // A solution object is meaningless without an instance.
            Instance instance;

            // The route matrix.
            Nilib::Matrixf adjacencyMatrix;
            float optimal_objective = 0.0;

            Solution(Instance const &inst, Nilib::Matrixf const &X, float const opt_objval);
            Solution(Instance const &inst, Nilib::Matrixf const &X);

            // Shuffle the solution.
            void random();

            static Solution emptySolution(Instance const &inst);
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
    
        // Pretty basic list<Route> VRP solution. 
        template<typename VRPinstance, typename Route>
        struct RouteSolution : public ALNS::Solution
        {
            VRPinstance instance;

            std::vector<Route> routes;

            // static RouteSolution random(size_t const nnodes, size_t const veh);
            virtual void draw(Nilib::Window &window) const = 0;
            
            // Inherited from ALNS::Solution. 
            virtual float objective() const = 0;
            virtual bool feasible() const = 0;
        };
    };
};

#endif