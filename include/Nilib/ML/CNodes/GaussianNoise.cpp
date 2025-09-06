#include "Nilib/ML/CNodes/GaussianNoise.h"

#include "Nilib/Math/LinALg.hpp"

using namespace Nilib;

GaussianNoise::GaussianNoise(CNode *input, float const mean = 0.0f, float const sigma = 0.0f)
    : input(input), mean(mean), sigma(sigma)
{
}

void GaussianNoise::evaluate()
{
    input->evaluate();
    this->value = input->value + Nilib::Matrixf::randn(input->value.rows(), input->value.cols(), mean, sigma);
}

void GaussianNoise::derive(Matrixf const &seed)
{
    input->derive(seed);
}
