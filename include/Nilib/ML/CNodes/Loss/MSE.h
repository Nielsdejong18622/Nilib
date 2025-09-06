#ifndef _CNODE_MSELOSS_H
#define _CNODE_MSELOSS_H

#include "Nilib/ML/CNodes/CNode.h"

namespace Nilib
{

    class MSELoss : public Nilib::CNode
    {
    public:
        MSELoss(CNode *prediction, CNode *target);

        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;

    private:
        CNode *prediction = nullptr;
        CNode *target = nullptr;
    };
}
#endif