
#ifndef _CNODE_GRAPHLAYER_H
#define _CNODE_GRAPHLAYER_H

#include "Nilib/ML/CNodes/CNode.h"
#include "Nilib/ML/CNodes/Activation/Sigmoid.h"

namespace Nilib
{

    // Takes a node feature vector X -> C where cij = MLP[xi xj]
    struct NN_outerdecoder : public CNode
    {
        CNode *X, *Wt, *Wb, *b;

        Nilib::Matrixf cache;
        // X, C_ij = sigmoid(xi * Wt + xj * Wb + b)
        NN_outerdecoder(CNode *X, CNode *Wt, CNode *Wb, CNode *b)
            : X(X), Wt(Wt), Wb(Wb), b(b)
        {
            // ASSERT(Wt->value.rows() == X->value.cols(), Wt->value.rows(), "!=", X->value.cols());
            // ASSERT(Wb->value.rows() == X->value.cols(), Wb->value.rows(), "!=", X->value.cols());
            // ASSERT(Wt->value.cols() == Wb->value.cols(), Wt->value.rows(), "!=", Wb->value.cols());
            // CORE_ASSERT(Wt->value.cols() == 1);
            // CORE_ASSERT(b->value.rows() == 1);
            // CORE_ASSERT(b->value.cols() == 1);
        }

        void evaluate()
        {
            X->evaluate();
            Wt->evaluate();
            Wb->evaluate();
            b->evaluate();
            size_t const n = X->value.rows();
            size_t const f = X->value.cols();
            // Init the output value.
            this->cache = Nilib::Matrixf(n, n);
            this->cache.print();

            for (size_t i = 0; i < n; i++)
            {
                for (size_t j = 0; j < n; j++)
                {
                    cache(i, j) = b->value(0, 0);
                    for (size_t k = 0; k < f; k++)
                    {
                        this->cache(i, j) += X->value(i, k) * Wt->value(k, 0);
                        this->cache(i, j) += X->value(j, k) * Wb->value(k, 0);
                    }
                }
            }
            this->value = this->cache;
            this->value.apply(Sigmoid::sigmoid);
            CORE_ASSERT(this->value.rows() == n && this->value.cols() == n);
            LOG_DEBUG("Evaluated NN_OUTERDECODER");
        }

        void derive(Nilib::Matrixf const &seedC)
        {
            LOG_DEBUG("Derived partialX!");
            size_t const n = X->value.rows(); // Number of nodes.
            size_t const f = X->value.cols(); // Input feature dim.
            Nilib::Matrixf partialX = Nilib::Matrixf::zeros(n, f);
            for (size_t i = 0; i < n; i++)
            {
                for (size_t j = 0; j < n; j++)
                {
                }
            }
            LOG_DEBUG("Derived partialX!");
            X->derive(partialX);
        }
    };

    struct DenseGraphLayer : public CNode
    {
        CNode *A, *input, *W;

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
            this->value = 0.2f * A->value * input->value * W->value + 0.8f * input2->value * Wself->value;
        }

        void derive(Nilib::Matrixf const &seed)
        {
            A->derive(seed * transpose(input->value * W->value));
            input->derive(transpose(A->value) * seed * transpose(W->value));
            W->derive(transpose(A->value * input->value) * seed);
            input2->derive(0.5f * seed * transpose(Wself->value));
            Wself->derive(0.5f * transpose(input2->value) * seed);
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
            float scalefactor = X->value.rows(); // * X->value.rows();
            this->value = X->value * transpose(X->value);
            this->value /= scalefactor;
        }

        void derive(Nilib::Matrixf const &seed)
        {
            CORE_ASSERT(seed.cols() == X->value.rows());
            CORE_ASSERT(X->value.rows() == seed.rows());
            float scalefactor = X->value.rows(); // * X->value.rows();
            X->derive(seed * X->value / scalefactor + Nilib::transpose(Nilib::transpose(X->value) * seed / scalefactor));
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