
#ifndef _CNODE_DENSELAYER_H
#define _CNODE_DENSELAYER_H

#include "Nilib/ML/CNodes/CNode.h"

namespace Nilib {

    struct DenseLayer : public CNode {
        CNode *input, *W, *b;


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
            input->evaluate();
            W->evaluate();
            b->evaluate();
            this->value = input->value * W->value + b->value;
        }

        void derive(Matrixf const &seed)
        {
            input->derive(seed * transpose(W->value));
            W->derive(transpose(input->value) * seed);
            b->derive(seed);
        }

    };

    
    struct DenseGraphLayer : public CNode {
        CNode *A, *input, *W, *b;

        DenseGraphLayer(CNode *A, CNode *input, CNode *W, CNode *b)
        : A(A), input(input), W(W), b(b)
        {    
        }

        void evaluate() 
        {
            A->evaluate();
            input->evaluate();
            W->evaluate();
            b->evaluate();
            this->value = A->value * input->value * W->value + b->value;
        }

        void derive(Nilib::Matrixf const &seed)
        {
            input->derive(transpose(A->value) * seed * transpose(W->value));
            A->derive(seed * transpose(input->value * W->value));
            W->derive(transpose(A->value * input->value) * seed);
            b->derive(seed);
        }

    };

}

#endif