
#ifndef _CNODE_GRAPHLAYER_H
#define _CNODE_GRAPHLAYER_H

#include "Nilib/ML/Models/MultiLayerPerceptron.hpp"
#include "Nilib/ML/Models/Perceptron.hpp"
#include "Nilib/ML/CNodes/Concat.h"
#include "Nilib/ML/CNodes/Activation/Tanh.h"

namespace Nilib
{

    // GCN.
    struct GCN : public CNode
    {
        CNode *A = nullptr;
        CNode *X = nullptr;
        Weight W;

        GCN(CNode *A, CNode *X, size_t colX, size_t outdim)
            : A(A), X(X), W(colX, outdim)
        {
            // CORE_ASSERT(X->value.cols() > 0);
            CORE_ASSERT(outdim > 0);
        }

        void evaluate()
        {
            CORE_ASSERT(A);
            CORE_ASSERT(X);
            A->evaluate();
            X->evaluate();
            W.evaluate();
            ASSERT(A->value.cols() == X->value.rows(), "AX undefined, coldim(A):", A->value.cols(), "rowdim(X):", X->value.rows());
            ASSERT(X->value.cols() == W.value.rows(), "XW undefined, coldim(X):", X->value.cols(), "rowdim(W):", W.value.rows());
            this->value = A->value * X->value * W.value;
        }

        void derive(Nilib::Matrixf const &seed)
        {
            // std::cerr << "Deriving GCN!" << seed.avg() << std::endl;
            CORE_ASSERT(A);
            CORE_ASSERT(X)
            CORE_ASSERT(seed.rows() == A->value.rows());
            CORE_ASSERT(seed.cols() == W.value.cols());
            A->derive(seed * transpose(X->value * W.value));
            X->derive(transpose(A->value) * seed * transpose(W.value));
            W.derive(transpose(A->value * X->value) * seed);
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

    // Flattens n dimensions into F (nxf) -> f (1xf)
    struct GraphPoolAverage : public CNode
    {
        CNode *X = nullptr;
        float cached_rows = 0.0f;

        GraphPoolAverage(CNode *X)
            : X(X)
        {
        }

        void evaluate()
        {
            CORE_ASSERT(X);
            X->evaluate();

            cached_rows = X->value.rows();
            this->value = Nilib::Matrixf::zeros(1, X->value.cols());
            for (size_t col_idx = 0; col_idx < X->value.cols(); col_idx++)
                for (size_t row_idx = 0; row_idx < cached_rows; row_idx++)
                    this->value(0, col_idx) += X->value(row_idx, col_idx);

            this->value /= cached_rows;
        }

        void derive(Nilib::Matrixf const &seed)
        {
            CORE_ASSERT(X);
            CORE_ASSERT(cached_rows > 0.0);
            CORE_ASSERT(seed.rows() == 1);
            Matrixf iota = Matrixf::ones(cached_rows, 1); // Already transposed.
            X->derive(iota * seed / cached_rows);
        }
    };
}

#endif