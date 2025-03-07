#include "Nilib/ML/CNodes/Input.h"

void Nilib::Input::set(Nilib::Matrixf const &value)
{
    this->value = value;
    if (this->partial.rows() != value.rows() || this->partial.cols() != value.cols())
        this->partial = Nilib::Matrixf::zeros(value.rows(), value.cols());
}

Nilib::Input::Input(Nilib::Matrixf const &value)
{
    set(value);
}

void Nilib::Input::evaluate()
{
}

void Nilib::Input::derive(Nilib::Matrixf const &seed)
{
    // if (partial.rows() == seed.rows() && partial.cols() == seed.cols())
    partial += seed;
}