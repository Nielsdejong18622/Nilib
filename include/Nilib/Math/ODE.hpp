#ifndef _ODE_HPP
#define _ODE_HPP

#include "Nilib/Math/Matrix.hpp"
#include "Nilib/Math/LinAlg.hpp"
#include "Nilib/Math/LinAlgFunctions.hpp"

namespace Nilib
{
    namespace ODE
    {
        // Observation, or point.
        using X = Nilib::Mat<1, 1>;
        //using X = Nilib::Matrixf;
        // Standard ODE solver framework.

        // A differential function.
        // y' = f(y, t), y(t_0) = y_0
        using ODEfunction = X (*)(X const &, float const t);

        std::vector<X> EulerMethod(ODEfunction fun, float const start, float const end, X const initial, float const timestep)
        {
            X currentval = initial;
            std::vector<X> results;

            results.push_back(currentval);
            size_t const iterations = (end - start) / timestep;
            float current_time = start;
            for (size_t iter = 0; iter < iterations; ++iter)
            {
                auto slope = fun(currentval, current_time);
                slope *= timestep;
                currentval += slope;
                results.push_back(currentval);
                current_time += timestep;
            }
            

            // Returns a vector of observations (or points) from [start, end, by = timestep(h)].
            
            return results;
        }

    }; // namespace ODE.

} // namespace Nilib

#endif