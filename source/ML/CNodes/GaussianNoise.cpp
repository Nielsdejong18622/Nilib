#include "Nilib/ML/CNodes/GaussianNoise.h"

#include "Nilib/Math/LinALg.hpp"

using namespace Nilib;

GaussianNoise::GaussianNoise(CNode *input, float const mean, float const sigma)
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
