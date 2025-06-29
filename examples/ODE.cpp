#include "Nilib/Logger/Log.hpp"
#include "Nilib/Logger/CSV.hpp"

#include "Nilib/Math/ODE.hpp"

float simpleODE(float const &X, float const t)
{
    // y'= y
    return X;
}

// Lotka-Volterra Predator-Prey equations.
Nilib::Vec2f predatorPrey(Nilib::Vec2f const &Y, float const t)
{
    // x'(t) = a x(t) - b * y(t) * x(t),
    // y'(t) = c y(t) - d y(t),
    // y(0) = known, x(0) = known
    Nilib::Vec2f deriv(Y);
    float a, b, c, d;
    a = 0.8;
    b = 1.2;
    c = 0.4;
    d = 0.4;

    deriv(0) = a * Y(0) - b * Y(0) * Y(1);
    deriv(1) = -c * Y(1) + d * Y(1) * Y(0);
    return deriv;
}

Nilib::Vec3f lorentzAttractor(Nilib::Vec3f const &Y, float const t)
{
    Nilib::Vec3f deriv(Y);
    float sigma = 10;
    float rho = 28;
    float beta = 8 / 3;

    // 0->x, 1->y, 2->z
    deriv(0) = sigma * (deriv(1) - deriv(0));
    deriv(1) = deriv(0) * (rho - deriv(2)) - deriv(1);
    deriv(2) = deriv(0) * deriv(1) - beta * deriv(2);

    return deriv;
}

int main()
{
    using namespace Nilib;
    using namespace Nilib::ODE;

    // Simple ODE y'(t) = y(t), y(0) = 9.
    float initval = 1;
    initval *= 9;

    auto res = Nilib::ODE::RungeKutta4<float>(&simpleODE, 0, 10, initval, 0.01);

    for (size_t idx = 0; idx < res.size(); ++idx)
    {
        auto ODEval = res[idx];
        auto realval = initval * exp(idx * 0.01);
        CSV("simpleODE.csv", idx, ODEval, realval);
    }

    // // Lotka-Volterra Predator-Prey equations.
    using Y = Nilib::Vec2f;

    Y initval1 = Y::ones();
    initval1(0) = 0.9;
    initval1(1) = 0.4;

    auto res_predatorprey = Nilib::ODE::RungeKutta4<Y>(&predatorPrey, 0, 100, initval1, 0.1);

    for (size_t idx = 0; idx < res_predatorprey.size(); ++idx)
    {
        auto prey = res_predatorprey[idx](0);
        auto predator = res_predatorprey[idx](1);
        float time = idx * 0.1;
        CSV("predatorprey.csv", time, prey, predator);
    }

    // // Lorentz-Attractor equations.
    Nilib::Vec3f initval_lorenz = Nilib::Vec3f::ones();
    initval_lorenz(0) = 0.9;
    initval_lorenz(1) = 0.0;
    initval_lorenz(2) = 0.0;

    auto res_lorentz = Nilib::ODE::RungeKutta4<Nilib::Vec3f>(&lorentzAttractor, 0, 1000, initval_lorenz, 0.001);

    for (size_t idx = 0; idx < res_lorentz.size(); ++idx)
    {
        auto x = res_lorentz[idx](0);
        auto y = res_lorentz[idx](1);
        auto z = res_lorentz[idx](2);
        float time = idx * 0.001;
        CSV("lorentz.csv", time, x, y, z);
    }

    return 0;
}