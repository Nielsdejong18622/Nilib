#include "Nilib/ML/Models/Perceptron.hpp"

Nilib::Perceptron::Perceptron(CNode *x, size_t const x_col)
    : MultilayerPerceptron(x, x_col, 1)
{
}
