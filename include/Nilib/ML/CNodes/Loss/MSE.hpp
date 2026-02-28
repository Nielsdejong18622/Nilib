#ifndef _CNODE_MSELOSS_H
#define _CNODE_MSELOSS_H

#include "Nilib/ML/CNodes/Activation/Square.hpp"
#include "Nilib/ML/CNodes/CNode.hpp"
#include "Nilib/ML/CNodes/Gates.hpp"
#include "Nilib/ML/CNodes/Layer/Pool.hpp"

namespace Nilib
{

class MSELoss : public Nilib::CNode
{
  public:
    MSELoss(CNode &prediction, CNode &target);

    void evaluate() override;
    void derive(Nilib::Matrixf const &seed) override;

  private:
    CNode &d_prediction;
    CNode &d_target;

    Minus d_mingate;
    Square d_square;
    MeanPool d_meanpool;
};
} // namespace Nilib
#endif