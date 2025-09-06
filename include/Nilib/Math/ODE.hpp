#ifndef _ODE_HPP
#define _ODE_HPP

#include "Nilib/Math/Matrix.hpp"
#include "Nilib/Math/LinAlg.hpp"
#include "Nilib/Math/LinALg.hpp"

namespace Nilib
{
    namespace ODE
    {
        // Observation, or point.
        // using Y = Nilib::Mat<1, 1>;
        // using X = Nilib::Matrixf;
        //  Standard ODE solver framework.

        // A differential function.
        // y' = f(y, t), y(t_0) = y_0
        // using ODEfunction = Y (*)(Y const &, float const t);

        template <typename Y = Nilib::Mat<1, 1>, typename ODEfunction = Y (*)(Y const &, float const t)>
        std::vector<Y> EulerMethod(ODEfunction fun, float const start, float const end, Y const initial, float const timestep)
        {
            CORE_ASSERT(start <= end);
            Y currentval = initial;
            std::vector<Y> results;

            size_t const iterations = (end - start) / timestep;
            results.reserve(iterations + 1);
            results.push_back(currentval);
            float current_time = start;
            for (size_t iter = 0; iter < iterations; ++iter)
            {
                auto m = fun(currentval, current_time);
                currentval += m * timestep;
                current_time += timestep;
                results.push_back(currentval);
            }

            // Returns a vector of observations (or points) from [start, end, by = timestep(h)].
            return results;
        }

        template <typename Y = Nilib::Mat<1, 1>, typename ODEfunction = Y (*)(Y const &, float const t)>
        std::vector<Y> RungeKutta4(ODEfunction fun, float const start, float const end, Y const initial, float const timestep)
        {
            CORE_ASSERT(start <= end);
            Y currentval = initial;
            std::vector<Y> results;

            size_t const iterations = (end - start) / timestep;
            results.reserve(iterations + 1);
            results.push_back(currentval);
            float current_time = start;
            for (size_t iter = 0; iter < iterations; ++iter)
            {
                auto k1 = fun(currentval, current_time);
                auto k2 = fun(currentval + timestep * k1 / 2.0f, current_time + timestep / 2.0f);
                auto k3 = fun(currentval + timestep * k2 / 2.0f, current_time + timestep / 2.0f);
                auto k4 = fun(currentval + k3 * timestep, current_time + timestep);

                auto m = (k1 + k4) / 6.0f + (k2 + k3) / 3.0f;

                currentval += m * timestep;
                current_time += timestep;
                results.push_back(currentval);
            }

            // Returns a vector of observations (or points) from [start, end, by = timestep(h)].
            return results;
        }

    }; // namespace ODE.

} // namespace Nilib

#endif