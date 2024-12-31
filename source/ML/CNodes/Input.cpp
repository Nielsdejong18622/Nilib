#include "Nilib/ML/CNodes/Input.h"

Nilib::Input::Input(Nilib::Matrixf const &value) 
{
    this->value = value;
    this->partial = Nilib::Matrixf::zeros(value.rows(), value.cols());
}

void Nilib::Input::evaluate()
{
}

void Nilib::Input::derive(Nilib::Matrixf const &seed)
{
    partial += seed;
}