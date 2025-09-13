#include "Nilib/ML/CNodes/Input.hpp"

using namespace Nilib;

Input::Input()
    : GNode(0, 0)
{
}

Input::Input(Nilib::Matrixf const &value)
    : GNode(value)
{
}

void Input::set(Nilib::Matrixf const &value)
{
    this->value = value;
    this->partial = value;
    this->partial.zero();
}
