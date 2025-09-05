#include "Nilib/ML/Models/MultiLayerPerceptron.hpp"

Nilib::MultilayerPerceptron::MultilayerPerceptron(CNode *x, size_t const x_col, size_t const neurons)
    : x(x),
      W(x_col, neurons),
      b(1, neurons)
{
}

void Nilib::MultilayerPerceptron::evaluate()
{
    CORE_ASSERT(x);
    x->evaluate();
    W.evaluate();
    b.evaluate();
    ASSERT(x->value.cols() == W.value.rows(), "xW undefined", x->value.cols(), "!=", W.value.rows());
    ASSERT(x->value.rows() == b.value.rows(), "xW + b undefined input of MultilayerPerceptron should be 1xinputdim", x->value.cols(), "!=", b.value.rows());
    ASSERT(W.value.cols() == b.value.cols(), "xW + b undefined input of MultilayerPerceptron should be 1xinputdim", W.value.cols(), "!=", b.value.cols());
    this->value = x->value * W.value + b.value;
}

void Nilib::MultilayerPerceptron::derive(Nilib::Matrixf const &seed)
{
    // LOG_DEBUG("Derive multilayer perceptron", this, "with seed:", seed);
    CORE_ASSERT(x);
    ASSERT(seed.rows() == b.value.rows(), "Output of MultilayerPerceptron is 1 x outputdim!");
    ASSERT(seed.cols() == b.value.cols(), "Output of MultilayerPerceptron is 1 x outputdim!");
    b.derive(seed);

    CORE_ASSERT(x->value.rows() == seed.rows());
    auto tmp = Nilib::transpose(x->value) * seed;
    CORE_ASSERT(tmp.rows() == W.value.rows());
    CORE_ASSERT(tmp.cols() == W.value.cols());
    W.derive(tmp);

    CORE_ASSERT(seed.cols() == W.value.cols());
    this->partial = seed * Nilib::transpose(W.value);
    CORE_ASSERT(this->partial.rows() == x->value.rows());
    CORE_ASSERT(this->partial.cols() == x->value.cols());
    x->derive(this->partial);
}
