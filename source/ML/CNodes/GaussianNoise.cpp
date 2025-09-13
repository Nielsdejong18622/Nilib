#include "Nilib/ML/CNodes/GaussianNoise.hpp"

#include "Nilib/Math/LinALg.hpp"

using namespace Nilib;

GaussianNoise::GaussianNoise(CNode &input, float const mean, float const sigma)
    : input(input), d_mean(mean), d_sigma(sigma)
{
}

void GaussianNoise::evaluate()
{
    input.evaluate();
    this->value = input.value + Nilib::Matrixf::randn(input.value.rows(), input.value.cols(), d_mean, d_sigma);
}

void GaussianNoise::derive(Matrixf const &seed)
{
    input.derive(seed);
}
