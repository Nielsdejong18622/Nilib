#ifndef _CNODE_REGULARIZER_H
#define _CNODE_REGULARIZER_H

#include "Nilib/ML/CNodes/CNode.h"
#include "Nilib/ML/CNodes/Param.h"

namespace Nilib
{

    struct Regularizer : public CNode
    {
        std::vector<CNode *> reg_nodes;
        Param *labda = nullptr; // Hyper parameter enalty term.

        Regularizer(std::initializer_list<CNode *> const &weights, Param *labda)
            : reg_nodes(weights), labda(labda)
        {
        }

        void evaluate() override
        {
            // Loop over all the weights and penalize their size using some penalty function.
            float penalty = 0;
            for (size_t wi = 0; wi < reg_nodes.size(); ++wi)
            {
                reg_nodes[wi]->evaluate();
                Matrixf tmp = reg_nodes[wi]->value; // Copy!
                tmp.apply(std::bind(&Regularizer::penalty, this, std::placeholders::_1, tmp));
                penalty += labda->value(0, 0) * tmp.sum();
            }
            this->value = Nilib::Matrixf(1, 1, {penalty});
        }

        void derive(Matrixf const &seed) override
        {
            for (size_t wi = 0; wi < reg_nodes.size(); ++wi)
            {
                Matrixf tmp = reg_nodes[wi]->value;
                tmp.apply(std::bind(&Regularizer::penalty_deriv, this, std::placeholders::_1, tmp));
                reg_nodes[wi]->derive(labda->value(0, 0) * tmp);
            }
        }

        virtual float penalty(float t, Matrixf const &w) = 0;
        virtual float penalty_deriv(float t, Matrixf const &w) = 0;
    };
}

#endif
