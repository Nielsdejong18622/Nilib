#include "Nilib/ML/CNodes/Input.hpp"

using namespace Nilib;

Input::Input()
    : GNode(0, 0)
{
}

Input::Input(Nilib::Matrixf const &val)
    : GNode(val)
{
}

void Input::set(Nilib::Matrixf const &val)
{
    this->value = val;
    this->partial = val;
    this->partial.zero();
}
