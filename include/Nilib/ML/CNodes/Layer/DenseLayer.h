
#ifndef _CNODE_DENSELAYER_H
#define _CNODE_DENSELAYER_H

#include "Nilib/ML/CNodes/CNode.h"

namespace Nilib
{

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

    struct DenseGraphLayer : public CNode
    {
        CNode *A, *input, *W;
        bool owning = false;

        DenseGraphLayer(CNode *A, CNode *input, CNode *W)
            : A(A), input(input), W(W)
        {
        }

        void evaluate()
        {
            A->evaluate();
            input->evaluate();
            W->evaluate();
            CORE_ASSERT(A->value.cols() == input->value.rows());
            CORE_ASSERT(input->value.cols() == W->value.rows());
            this->value = A->value * input->value * W->value;
        }

        void derive(Nilib::Matrixf const &seed)
        {
            A->derive(seed * transpose(input->value * W->value));
            input->derive(transpose(A->value) * seed * transpose(W->value));
            W->derive(transpose(A->value * input->value) * seed);
        }
    };

    struct OuterProductDecoder : public CNode
    {
        CNode *X;

        OuterProductDecoder(CNode *X)
            : X(X)
        {
        }

        void evaluate()
        {
            X->evaluate();
            float scalefactor = X->value.rows() * X->value.rows();
            this->value = X->value * transpose(X->value) / scalefactor;
        }

        void derive(Nilib::Matrixf const &seed)
        {
            CORE_ASSERT(seed.cols() == X->value.rows());
            CORE_ASSERT(X->value.rows() == seed.rows());
            float scalefactor = X->value.rows() * X->value.rows();
            X->derive(seed * X->value / scalefactor + Nilib::transpose(Nilib::transpose(X->value) * seed / scalefactor));
            //X->derive(Nilib::transpose(Nilib::transpose(X->value) * seed) + seed * Nilib::transpose(X->value));
        }
    };

    struct GraphPoolAverage : public CNode
    {
        CNode *X;
        Matrixf iota;

        GraphPoolAverage(CNode *X)
            : X(X)
        {
        }

        void evaluate()
        {
            X->evaluate();
            iota = Matrixf::all(1, X->value.rows(), 1.0f);
            this->value = (1.0f / X->value.rows()) * iota * X->value;
        }

        void derive(Nilib::Matrixf const &seed)
        {
            CORE_ASSERT(X->value.rows() > 0);
            X->derive((1.0f / X->value.rows()) * Nilib::transpose(iota) * seed);
        }
    };

}

#endif