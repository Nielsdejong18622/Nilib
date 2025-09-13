#include "Nilib/ML/CNodes/Loss/MSE.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

MSELoss::MSELoss(CNode &prediction, CNode &target)
    : prediction(prediction),
      target(target),
      mingate(prediction, target),
      square(mingate),
      meanpool(square)
{
}

void MSELoss::evaluate()
{
    meanpool.evaluate();
    this->value = meanpool.value;
}

void MSELoss::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.rows() == 1);
    CORE_ASSERT(seed.cols() == 1);
    meanpool.derive(seed);
}