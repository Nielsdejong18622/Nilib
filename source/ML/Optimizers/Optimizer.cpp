#include "Nilib/ML/Optimizers/Optimizer.hpp"
#include "Nilib/ML/CNodes/Weight.h"

Nilib::Optimizer::Optimizer(std::vector<Weight *> weights)
    : weights(weights)
{
}

void Nilib::Optimizer::updateGrad() const
{
    // To be overridden.
}

void Nilib::Optimizer::zeroGrad() const
{
    for (Weight *w : weights)
        w->partial.zero();
}

void Nilib::Optimizer::save(std::string const &filename) const
{
    Serializer writer(filename);
    for (Weight const *w : weights)
        writer.writeMatrix(w->value);
}

void Nilib::Optimizer::load(std::string const &filename) const
{
    Deserializer loader(filename);
    for (Weight *w : weights)
        loader.readMatrix(w->value);
}

bool Nilib::Optimizer::checkgradients() const
{
    for (Weight const *w : weights)
        if (w->partial.rows() == 0 || w->partial.cols() == 0)
            return false;
    return true;
}
