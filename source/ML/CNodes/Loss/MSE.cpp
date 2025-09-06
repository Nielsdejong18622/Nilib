#include "Nilib/ML/CNodes/Loss/MSE.h"
#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

MSELoss::MSELoss(CNode *prediction, CNode *target)
    : prediction(prediction), target(target) {}

void MSELoss::evaluate()
{
    CORE_ASSERT(prediction && target)
    target->evaluate();
    prediction->evaluate();
    ASSERT(target->value.rows() == prediction->value.rows() && target->value.cols() == prediction->value.cols(),
           target->value.rows(), "!=", prediction->value.rows(), "or", target->value.cols(), "!=", prediction->value.cols())
    auto Error = prediction->value - target->value;
    this->value = 0.5f * Nilib::hadamar(Error, Error);
}

void MSELoss::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(prediction && target)
    auto tmp = Nilib::hadamar(prediction->value - target->value, seed);
    prediction->derive(tmp);
    target->derive(-1.0f * tmp);
}