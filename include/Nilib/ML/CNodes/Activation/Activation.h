
#ifndef _CNODE_ACTVIVATION_H
#define _CNODE_ACTVIVATION_H

#include "Nilib/ML/CNodes/CNode.h"

namespace Nilib {

    struct Activation : public Nilib::CNode {
        CNode *input;
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
            input->evaluate();
            this->value = input->value; // Copy the input. 
            this->value.apply(actfun);
        }

        void derive(Nilib::Matrixf const &seed)
        {
            auto tmp = input->value; // Get a copy of the input. 
            tmp.apply(deriv);
            input->derive(Nilib::hadamar(seed, tmp));
        }
    };
}
#endif

