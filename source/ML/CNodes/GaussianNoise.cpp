#include "Nilib/ML/CNodes/GaussianNoise.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

GaussianNoise::GaussianNoise(CNode &input, float const mean, float const sigma)
    : d_input(input), d_mean(mean), d_sigma(sigma)
{
}

void GaussianNoise::evaluate()
{
    d_input.evaluate();
    this->value = d_input.value + Nilib::Matrixf::randn(d_input.value.rows(), d_input.value.cols(), d_mean, d_sigma);
}

void GaussianNoise::derive(Matrixf const &seed)
{
    d_input.derive(seed);
}
