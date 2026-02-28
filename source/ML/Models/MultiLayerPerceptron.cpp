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
    ASSERT(d_x.value.cols() == d_W.value.rows(), "xW undefined", d_x.value, "!=", d_W.value, "Weight:", &d_W);
    ASSERT(d_x.value.rows() == d_b.value.rows(), "xW + b undefined input of MultilayerPerceptron should be 1xinputdim", d_x.value, "!=", d_b.value);
    ASSERT(d_W.value.cols() == d_b.value.cols(), "xW + b undefined input of MultilayerPerceptron should be 1xinputdim", d_W.value, "!=", d_b.value);
    this->value = d_x.value * d_W.value + d_b.value;
}

void Nilib::MultilayerPerceptron::derive(Nilib::Matrixf const &seed)
{
    ASSERT(seed.rows() == d_b.value.rows(), "Output of MultilayerPerceptron is 1 x outputdim!");
    ASSERT(seed.cols() == d_b.value.cols(), "Output of MultilayerPerceptron is 1 x outputdim!");
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
