
#ifndef _CNODE_GRAPHLAYER_H
#define _CNODE_GRAPHLAYER_H

#include "Nilib/ML/CNodes/CNode.h"
#include "Nilib/ML/CNodes/Activation/Relu.h"

namespace Nilib
{

    // Takes a node feature vector X -> C where cij = MLP[xi xj]
    struct NN_outerdecoder : public CNode
    {
        CNode *X;
        Weight *Wt, *Wb, *b;

        Nilib::Matrixf cache;
        // X, C_ij = sigmoid(xi * Wt + xj * Wb + b)
        NN_outerdecoder(CNode *X, Weight *Wt, Weight *Wb, Weight *b)
            : X(X), Wt(Wt), Wb(Wb), b(b)
        {
        }

        void evaluate()
        {
            X->evaluate();
            Wt->evaluate();
            Wb->evaluate();
            b->evaluate();

            ASSERT(Wt->value.rows() == X->value.cols(), Wt->value.rows(), "!=", X->value.cols());
            ASSERT(Wb->value.rows() == X->value.cols(), Wb->value.rows(), "!=", X->value.cols());
            ASSERT(Wt->value.cols() == Wb->value.cols(), Wt->value.rows(), "!=", Wb->value.cols());
            CORE_ASSERT(Wt->value.cols() == 1);
            CORE_ASSERT(b->value.rows() == 1);
            CORE_ASSERT(b->value.cols() == 1);

            size_t const n = X->value.rows();
            size_t const f = X->value.cols();
            // Init the output value.
            this->cache = Nilib::Matrixf(n, n);

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
            // this->value.apply(std::bind(Relu::relu, std::placeholders::_1, 0.01f));
            CORE_ASSERT(this->value.rows() == n && this->value.cols() == n);
        }

        void derive(Nilib::Matrixf const &seedC)
        {
            // LOG_DEBUG("Deriving partialX!");
            size_t const n = X->value.rows(); // Number of nodes.
            size_t const f = X->value.cols(); // Input feature dim.

            // this->cache.apply(std::bind(Relu::relu_deriv, std::placeholders::_1, 0.01f));

            for (size_t i = 0; i < n; i++)
            {
                Nilib::Matrixf deriv_x(n, f);
                for (size_t j = 0; j < n; j++)
                {
                    Nilib::Matrixf derivb{seedC(i, j) * this->cache(i, j)};
                    Nilib::Matrixf deriv_wt(f, 1);
                    Nilib::Matrixf deriv_wb(f, 1);
                    this->b->derive(derivb);

                    for (size_t k = 0; k < f; k++)
                    {
                        deriv_x(i, k) = seedC(i, j) * this->cache(i, j) * Wb->value(k);
                        deriv_x(j, k) = seedC(i, j) * this->cache(i, j) * Wb->value(k);
                        deriv_wt(k, 0) = X->value(i, k) * seedC(i, j) * this->cache(i, j);
                        deriv_wb(k, 0) = X->value(j, k) * seedC(i, j) * this->cache(i, j);
                    }

                    Wt->derive(deriv_wt);
                    Wb->derive(deriv_wb);
                }
                X->derive(deriv_x);
            }
            // LOG_DEBUG("Derived partialX!");
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
            ASSERT(A->value.cols() == input->value.rows(), "AX undefined coldim(A):", A->value.cols(), "rowdim(X):", input->value.rows());
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
            this->value = 0.2 * A->value * input->value * W->value + 0.8 * input2->value * Wself->value;
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
            float scalefactor = X->value.rows(); // * X->value.rows();
            this->value = X->value * transpose(X->value) / scalefactor;
        }

        void derive(Nilib::Matrixf const &seed)
        {
            CORE_ASSERT(seed.cols() == X->value.rows());
            CORE_ASSERT(X->value.rows() == seed.rows());
            float scalefactor = X->value.rows(); // * X->value.rows();
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