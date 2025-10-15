#include "Nilib/ML/Optimizers/Optimizer.hpp"

Nilib::Optimizer::Optimizer(Module *modul)
{
    CORE_ASSERT(modul != nullptr);
    modul->learnables(weights);
    initWeights();
    zeroGrad();
}

Nilib::Optimizer::Optimizer(Weightptrs const &weights)
    : weights(weights)
{
    initWeights();
    zeroGrad();
}

void Nilib::Optimizer::updateGrad(float const multiplier)
{
    // To be overridden.
    CORE_ASSERT(false);
}

void Nilib::Optimizer::zeroGrad() const
{
    for (Weight *w : weights)
        w->partial.zero();
}

/// @brief Generate weights using N(0.0f, 1 / n_l-1), where n_l-1 is the col dim of the previous layer.
void Nilib::Optimizer::initWeights() const
{
    // CORE_ASSERT(weights.size() > 0);
    // CORE_ASSERT(weights[0] != nullptr);
    // float prev_layer = weights[0]->value.cols();
    float prev_layer = 0.0f;
    for (Weight *w : weights)
    {
        w->partial.setrandn(0.0f, 1.0f / (1.0f + prev_layer));
        // prev_layer = w->value.cols();
    }
}

void Nilib::Optimizer::printWeights() const
{
    CORE_ASSERT(checkgradients());
    for (Weight const *w : weights)
        LOG_DEBUG()
            << "Weight " << w << ' ' << w->value << std::setprecision(4)
            << " value " << w->value.avg() << '(' << w->value.stddev() << ')'
            << " partial " << w->partial.avg() << '(' << w->partial.stddev() << ')' << '\n';
}

void Nilib::Optimizer::save(std::filesystem::path const &file) const
{
    Serializer writer(file.string());
    for (Weight const *w : weights)
        writer.writeMatrix(w->value);
}

void Nilib::Optimizer::load(std::filesystem::path const &file) const
{
    Deserializer loader(file.string());
    if (loader.opened())
    {
        LOG_WARNING("Loading weights from", file);
        for (Weight *w : weights)
        {
            // size_t row = w->value.rows(), col = w->value.cols();
            loader.readMatrix(w->value);
            // if ((row != w->value.rows() || col == w->value.cols()))
            // {
            //     throw std::runtime_error("Weight matrix from disk incompatible with existing weight matrix!");
            // }
        }
    }
}

bool Nilib::Optimizer::checkgradients() const
{
    for (Weight const *w : weights)
    {
        if (w == nullptr || w->partial.rows() == 0 || w->partial.cols() == 0)
            return false;
        if ((w->value.rows() != w->partial.rows()) || (w->value.rows() != w->partial.rows()))
            return false;
    }
    return true;
}
