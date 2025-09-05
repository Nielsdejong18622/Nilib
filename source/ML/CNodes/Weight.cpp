#include "Nilib/ML/CNodes/Weight.h"

Nilib::Weight::Weight(size_t n, size_t m)
    : Input(Matrixf::randn(n, m, 0.0, 1.0))
{
}