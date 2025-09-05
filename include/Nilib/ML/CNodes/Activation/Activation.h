
#ifndef _CNODE_ACTVIVATION_H
#define _CNODE_ACTVIVATION_H

#include "Nilib/ML/CNodes/CNode.h"

namespace Nilib
{

    struct Activation : public Nilib::CNode
    {
        CNode *input = nullptr;
        using ActFun = std::function<float(float)>;
        using ActFunDeriv = std::function<float(float)>;
        ActFun actfun;
        ActFunDeriv deriv;

        Activation(CNode *input, ActFun const &fun, ActFunDeriv const &deriv)
            : input(input), actfun(fun), deriv(deriv)
        {
        }

        void evaluate()
        {
            CORE_ASSERT(input);
            input->evaluate();
            this->value = input->value;
            this->value.apply(actfun);
        }

        void derive(Nilib::Matrixf const &seed)
        {
            // LOG_DEBUG("Deriving activaition function!", this);
            CORE_ASSERT(input);
            CORE_ASSERT(seed.rows() == input->value.rows());
            CORE_ASSERT(seed.cols() == input->value.cols());
            auto tmp = input->value; // Get a copy of the input.
            tmp.apply(deriv);
            // LOG_DEBUG("Deriving activaition function2!", this);
            input->derive(Nilib::hadamar(seed, tmp));
        }
    };
}
#endif