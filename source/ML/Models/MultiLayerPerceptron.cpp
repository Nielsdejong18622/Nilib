#include "Nilib/ML/Models/MultiLayerPerceptron.hpp"

#include "Nilib/Math/LinAlg.hpp"

Nilib::MultilayerPerceptron::MultilayerPerceptron(CNode &x, size_t const x_col, size_t const neurons)
    : d_x(x),
      d_W(x_col, neurons),
      d_b(1, neurons)
{
}

void Nilib::MultilayerPerceptron::learnables(Weights &add)
{
    add.push_back(&d_W);
    add.push_back(&d_b);
}

void Nilib::MultilayerPerceptron::evaluate()
{
    d_x.evaluate();
    d_W.evaluate();
    d_b.evaluate();
    CORE_ASSERT(d_x.value.cols() == d_W.value.rows());
    CORE_ASSERT(d_x.value.rows() == d_b.value.rows());
    CORE_ASSERT(d_W.value.cols() == d_b.value.cols());
    this->value = d_x.value * d_W.value + d_b.value;
}

void Nilib::MultilayerPerceptron::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.rows() == d_b.value.rows());
    CORE_ASSERT(seed.cols() == d_b.value.cols());
    d_b.derive(seed);

    CORE_ASSERT(d_x.value.rows() == seed.rows());
    auto tmp = Nilib::transpose(d_x.value) * seed;
    CORE_ASSERT(tmp.rows() == d_W.value.rows());
    CORE_ASSERT(tmp.cols() == d_W.value.cols());
    d_W.derive(tmp);

    CORE_ASSERT(seed.cols() == d_W.value.cols());
    auto partial = seed * Nilib::transpose(d_W.value);
    CORE_ASSERT(partial.rows() == d_x.value.rows());
    CORE_ASSERT(partial.cols() == d_x.value.cols());
    d_x.derive(partial);
}
