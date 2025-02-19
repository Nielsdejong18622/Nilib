
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

    // Highway GCN.
    struct DenseGraphLayer2 : public CNode
    {
        CNode *A, *input, *input2, *W, *Wself;
        bool owning = false;

        DenseGraphLayer2(CNode *A, CNode *input, CNode *input2, CNode *W, CNode *Wself)
            : A(A), input(input), input2(input2), W(W), Wself(Wself)
        {
        }

        void evaluate()
        {
            A->evaluate();
            input->evaluate();
            input2->evaluate();
            W->evaluate();
            Wself->evaluate();

            CORE_ASSERT(A->value.cols() == input->value.rows());
            CORE_ASSERT(input->value.cols() == W->value.rows());
            CORE_ASSERT(input2->value.cols() == Wself->value.rows());
            this->value = 0.5 * A->value * input->value * W->value + 0.5 * input2->value * Wself->value;
        }

        void derive(Nilib::Matrixf const &seed)
        {
            A->derive(seed * transpose(input->value * W->value));
            input->derive(transpose(A->value) * seed * transpose(W->value));
            W->derive(transpose(A->value * input->value) * seed);
            input2->derive(0.5 * seed * transpose(Wself->value));
            Wself->derive(0.5 * transpose(input2->value) * seed);
        }
    };

    struct OuterProductDecoder2 : public CNode
    {
        CNode *X, *W;

        OuterProductDecoder2(CNode *X, CNode *W)
            : X(X), W(W)
        {
        }

        void evaluate()
        {
            X->evaluate();
            W->evaluate();
            CORE_ASSERT(X->value.cols() == W->value.rows());
            CORE_ASSERT(X->value.cols() == W->value.cols());
            float const scalefactor = X->value.rows() * X->value.rows();
            this->value = X->value * W->value * Nilib::transpose(X->value) / scalefactor;
            if (RNG::prob() < 0.00001)
            {
                LOG_DEBUG("Covar W", W->value, W->value.sum());
            }
        }

        void derive(Nilib::Matrixf const &seed)
        {
            CORE_ASSERT(seed.cols() == X->value.rows());
            CORE_ASSERT(X->value.rows() == seed.rows());
            float const scalefactor = X->value.rows() * X->value.rows();
            X->derive(seed * X->value * Nilib::transpose(W->value) / scalefactor + Nilib::transpose(seed) * X->value * W->value / scalefactor);
            W->derive(Nilib::transpose(X->value) * seed * X->value / scalefactor);
            // X->derive(Nilib::transpose(Nilib::transpose(X->value) * seed) + seed * Nilib::transpose(X->value));
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
            // X->derive(Nilib::transpose(Nilib::transpose(X->value) * seed) + seed * Nilib::transpose(X->value));
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