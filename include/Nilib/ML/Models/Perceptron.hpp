#ifndef _PERCEPTRON_HPP
#define _PERCEPTRON_HPP

#include "Nilib/ML/Models/MultiLayerPerceptron.hpp"

namespace Nilib
{
    struct Perceptron : public MultilayerPerceptron
    {
        Perceptron(CNode *x, size_t x_col);
    };
} // namespace Nilib

#endif