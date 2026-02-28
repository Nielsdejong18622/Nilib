#include "Nilib/ML/CNodes/Loss/MSE.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

MSELoss::MSELoss(CNode &prediction, CNode &target)
    : d_prediction(prediction), d_target(target), d_mingate(prediction, target), d_square(d_mingate),
      d_meanpool(d_square)
{
}

void MSELoss::evaluate()
{
    d_meanpool.evaluate();
    this->value = d_meanpool.value;
}

void MSELoss::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.rows() == 1);
    CORE_ASSERT(seed.cols() == 1);
    d_meanpool.derive(seed);
}