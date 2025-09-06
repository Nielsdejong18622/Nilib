
#ifndef _CNODE_ACTVIVATION_H
#define _CNODE_ACTVIVATION_H

#include "Nilib/ML/CNodes/CNode.h"
#include "Nilib/Math/LinALg.hpp"

namespace Nilib
{
    template <typename ActFun>
    struct Activation : public Nilib::CNode
    {
        CNode *input = nullptr;

        Activation(CNode *input)
            : input(input) {}

        void evaluate() override
        {
            CORE_ASSERT(input);
            input->evaluate();
            this->value = input->value;
            this->value.apply(ActFun::evaluate);
        }

        void derive(Nilib::Matrixf const &seed) override
        {
            CORE_ASSERT(input);
            CORE_ASSERT(seed.rows() == input->value.rows());
            CORE_ASSERT(seed.cols() == input->value.cols());

            input->value.apply(ActFun::derivative);
            input->derive(Nilib::hadamar(seed, input->value));
        }
    };

}
#endif