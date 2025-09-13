#ifndef _CNODE_MSELOSS_H
#define _CNODE_MSELOSS_H

#include "Nilib/ML/CNodes/CNode.hpp"
#include "Nilib/ML/CNodes/Gates.hpp"
#include "Nilib/ML/CNodes/Activation/Square.hpp"
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
        CNode &prediction;
        CNode &target;

        Minus mingate;
        Square square;
        MeanPool meanpool;
    };
}
#endif