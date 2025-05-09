
#ifndef _CNODE_MSELOSS_H
#define _CNODE_MSELOSS_H
// Well known loss functions.

#include "Nilib/ML/CNodes/CNode.h"

namespace Nilib
{

    struct MSELoss : public Nilib::CNode
    {
        CNode *prediction = nullptr;
        CNode *target = nullptr;

        MSELoss(CNode *prediction, CNode *target)
            : prediction(prediction), target(target) {}

        void evaluate()
        {
            // CORE_ASSERT(prediction && target)
            target->evaluate();
            prediction->evaluate();
            auto Error = prediction->value - target->value;
            this->value = 0.5 * Nilib::hadamar(Error, Error);
        }

        void derive(Nilib::Matrixf const &seed)
        {
            // CORE_ASSERT(prediction && target)
            auto tmp = Nilib::hadamar(prediction->value - target->value, seed);
            prediction->derive(tmp);
            target->derive(-1 * tmp);
        }
    };
}
#endif