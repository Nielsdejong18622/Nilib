
#ifndef _CNODE_GRAPHLAYER_H
#define _CNODE_GRAPHLAYER_H

#include "Nilib/ML/CNodes/CNode.h"
#include "Nilib/ML/CNodes/Activation/Relu.h"

namespace Nilib
{

    // Takes a node feature vector sigmoid(AXW) -> X', G, e -> F where fij = MLP([x_i, x_j, e_ij])
    // x_i [xpos, ypos, isdummy, isorder/courier, if courier then #orders_to`, ]
    // e_ij = [distance, manhattan distance, ]
    class NN_outerdecoderMLP : public CNode
    {
        Input x_i;
        Input x_j;
        Input e_ij;
        Rbind x_i_xj;
        Rbind features;
        Sigmoid sig;

    public:
        CNode *X = nullptr;
        MultilayerPerceptron mlp;
        Perceptron pp;

        // X, C_ij = sigmoid(xi * Wt + xj * Wb + b)
        NN_outerdecoderMLP(CNode *X, size_t const neurons)
            : X(X),
              x_i_xj(&x_i, &x_j), features(&x_i_xj, &e_ij),
              mlp(&features, neurons, neurons),
              sig(&mlp),
              pp(&sig, neurons)
        {
        }

        void evaluate()
        {
            // LOG_DEBUG("NN_outer MLP!");
            CORE_ASSERT(X);
            X->evaluate();
            // mlp.evaluate();
            size_t const n = X->value.rows();
            size_t const f = X->value.cols();

            CORE_ASSERT(n > 0);
            CORE_ASSERT(f > 0);
            // Init the output value.
            this->value = Nilib::Matrixf(n, n);

            for (size_t i = 0; i < n; i++)
            {
                for (size_t j = 0; j < n; j++)
                {
                    Nilib::Matrixf xi(1, f);
                    Nilib::Matrixf xj(1, f);
                    Nilib::Matrixf eij(1, 2);
                    // Euclidean Distance.
                    eij(0) = Nilib::euclidean(Vec2f{X->value(i, 2), X->value(i, 3)}, Vec2f{X->value(j, 2), X->value(j, 3)});
                    // Is order-courier or courier-order?
                    eij(1) = (X->value(i, 1) > 0.5f | X->value(j, 1) > 0.5f);
                    for (size_t k = 0; k < f; k++)
                    {
                        xi(k) = X->value(i, k);
                        xj(k) = X->value(j, k);
                    }

                    x_i.set(xi);
                    x_j.set(xj);
                    e_ij.set(eij);
                    pp.evaluate();
                    CORE_ASSERT(pp.value.size() > 0);
                    CORE_ASSERT(pp.value.cols() == 1);
                    CORE_ASSERT(pp.value.rows() == 1);
                    this->value(i, j) = pp.value(0);
                }
            }
            // this->value.apply(std::bind(Relu::relu, std::placeholders::_1, 0.01f));
            CORE_ASSERT(this->value.rows() == n && this->value.cols() == n);
        }

        void derive(Nilib::Matrixf const &seedC)
        {
            CORE_ASSERT(X);
            size_t const n = X->value.rows(); // Number of nodes.
            size_t const f = X->value.cols(); // Input feature dim.

            CORE_ASSERT(n > 0);
            CORE_ASSERT(f > 3);

            Nilib::Matrixf deriv_x = Nilib::Matrixf::zeros(n, f);
            for (size_t i = 0; i < n; i++)
            {
                for (size_t j = 0; j < n; j++)
                {
                    // Get the i-th and j-th row.
                    Nilib::Matrixf xi(1, f);
                    Nilib::Matrixf xj(1, f);
                    Nilib::Matrixf eij(1, 2);
                    // Euclidean Distance.
                    eij(0) = Nilib::euclidean(Vec2f{X->value(i, 2), X->value(i, 3)}, Vec2f{X->value(j, 2), X->value(j, 3)});
                    // Is order-courier or courier-order?
                    eij(1) = (X->value(i, 1) > 0.5f | X->value(j, 1) > 0.5f);
                    for (size_t k = 0; k < f; k++)
                    {
                        xi(k) = X->value(i, k);
                        xj(k) = X->value(j, k);
                    }

                    x_i.set(xi);
                    x_j.set(xj);
                    e_ij.set(eij);
                    pp.evaluate();
                    CORE_ASSERT(pp.value.size() > 0);
                    CORE_ASSERT(pp.value.cols() == 1);
                    CORE_ASSERT(pp.value.rows() == 1);
                    pp.derive({seedC(i, j)});

                    // Add the result to the deriv.
                    CORE_ASSERT(x_i.partial.rows() == 1);
                    CORE_ASSERT(x_j.partial.rows() == 1);
                    CORE_ASSERT(x_j.partial.cols() == f);
                    CORE_ASSERT(x_i.partial.cols() == f);
                    for (size_t k = 0; k < f; k++)
                    {
                        deriv_x(i, k) += x_i.partial(0, k);
                        deriv_x(j, k) += x_j.partial(0, k);
                    }
                }
            }
            // LOG_DEBUG("Derived NN_outerDecoderMLP!");
            X->derive(deriv_x);
        }
    };

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
            // std::cerr << "Deriving GCN!" << std::endl;
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