
#ifndef _CNODE_DENSELAYER_H
#define _CNODE_DENSELAYER_H

#include "Nilib/ML/CNodes/CNode.h"

namespace Nilib
{

    struct Linear : public CNode
    {
        CNode *input = nullptr, *W = nullptr;

        Linear(CNode *input, CNode *W)
            : input(input), W(W)
        {
        }

        void evaluate()
        {
            CORE_ASSERT(input && W);
            input->evaluate();
            W->evaluate();
            this->value = input->value * W->value;
        }

        void derive(Matrixf const &seed)
        {
            CORE_ASSERT(input && W);
            input->derive(seed * transpose(W->value));
            W->derive(transpose(input->value) * seed);
        }
    };

    struct DenseLayer : public CNode
    {
        CNode *input = nullptr, *W = nullptr, *b = nullptr;

        DenseLayer(CNode *input, size_t neurons)
            : input(input)
        {
            ASSERT(false, "Not implemented yet!");
        }

        DenseLayer(CNode *input, CNode *W, CNode *b)
            : input(input), W(W), b(b)
        {
        }

        void evaluate()
        {
            CORE_ASSERT(input && W && b);
            input->evaluate();
            W->evaluate();
            b->evaluate();
            this->value = input->value * W->value + b->value;
        }

        void derive(Matrixf const &seed)
        {
            CORE_ASSERT(input && W && b);
            input->derive(seed * transpose(W->value));
            W->derive(transpose(input->value) * seed);
            b->derive(seed);
        }
    };

}

#endif