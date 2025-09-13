#include "Nilib/ML/Models/MultiLayerPerceptron.hpp"

#include "Nilib/Math/LinAlg.hpp"

Nilib::MultilayerPerceptron::MultilayerPerceptron(CNode &x, size_t const x_col, size_t const neurons)
    : x(x),
      W(x_col, neurons),
      b(1, neurons)
{
}

void Nilib::MultilayerPerceptron::learnables(Weights &add)
{
    add.push_back(&W);
    add.push_back(&b);
}

void Nilib::MultilayerPerceptron::evaluate()
{
    x.evaluate();
    W.evaluate();
    b.evaluate();
    ASSERT(x.value.cols() == W.value.rows(), "xW undefined", x.value, "!=", W.value);
    ASSERT(x.value.rows() == b.value.rows(), "xW + b undefined input of MultilayerPerceptron should be 1xinputdim", x.value, "!=", b.value);
    ASSERT(W.value.cols() == b.value.cols(), "xW + b undefined input of MultilayerPerceptron should be 1xinputdim", W.value, "!=", b.value);
    this->value = x.value * W.value + b.value;
}

void Nilib::MultilayerPerceptron::derive(Nilib::Matrixf const &seed)
{
    ASSERT(seed.rows() == b.value.rows(), "Output of MultilayerPerceptron is 1 x outputdim!");
    ASSERT(seed.cols() == b.value.cols(), "Output of MultilayerPerceptron is 1 x outputdim!");
    b.derive(seed);

    CORE_ASSERT(x.value.rows() == seed.rows());
    auto tmp = Nilib::transpose(x.value) * seed;
    CORE_ASSERT(tmp.rows() == W.value.rows());
    CORE_ASSERT(tmp.cols() == W.value.cols());
    W.derive(tmp);

    CORE_ASSERT(seed.cols() == W.value.cols());
    auto partial = seed * Nilib::transpose(W.value);
    CORE_ASSERT(partial.rows() == x.value.rows());
    CORE_ASSERT(partial.cols() == x.value.cols());
    x.derive(partial);
}
