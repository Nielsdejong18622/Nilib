#include "Nilib/ML/CNodes/GNode.hpp"

Nilib::GNode::GNode(Nilib::Matrixf const &value)
{
    this->value = value;
    this->partial = Nilib::Matrixf::zeros(value.rows(), value.cols());
}

Nilib::GNode::GNode(size_t const rows, size_t const cols)
{
    this->value = Nilib::Matrixf::randn(rows, cols, 0.0f, 1.0f);
    this->partial = Nilib::Matrixf::zeros(rows, cols);
}

void Nilib::GNode::evaluate()
{
}

void Nilib::GNode::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(partial.rows() == seed.rows());
    CORE_ASSERT(partial.cols() == seed.cols());
    partial += seed;
}