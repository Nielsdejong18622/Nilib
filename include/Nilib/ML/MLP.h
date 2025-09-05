#ifndef _ML_MLP_H
#define _ML_MLP_H

#include "Nilib/ML/Model.h"
#include "Nilib/ML/CNodes/CNode.h"
#include "Nilib/ML/CNodes/Weight.h"
#include "Nilib/ML/CNodes/Plus.h"
#include "Nilib/ML/CNodes/Param.h"
#include "Nilib/ML/Cnodes/Layer/DenseLayer.h"

#include "Nilib/ML/Cnodes/Activation/Relu.h"
#include "Nilib/ML/Cnodes/Activation/Sigmoid.h"
#include "Nilib/ML/Cnodes/Activation/Tanh.h"

#include "Nilib/ML/Cnodes/Loss/MSE.h"
#include "Nilib/ML/Cnodes/Regularizer/L2.h"

namespace Nilib
{

    struct Perceptron : public Input
    {
        CNode *x = nullptr;
        Weight W;
        Weight b;

        Perceptron(CNode *x, size_t x_col)
            : x(x),
              W(x_col, 1),
              b(1, 1)
        {
        }

        void evaluate() override
        {
            CORE_ASSERT(x);
            x->evaluate();
            W.evaluate();
            b.evaluate();
            ASSERT(x->value.cols() == W.value.rows(), "xW undefined", x->value.cols(), "!=", W.value.rows());
            ASSERT(x->value.rows() == 1, "xW + b undefined input of Perceptron should be 1xinputdim", x->value.cols(), "!=", b.value.rows());
            this->value = x->value * W.value + b.value;
        }

        void derive(Nilib::Matrixf const &seed) override
        {
            ASSERT(seed.rows() == 1, "Output of perceptron is 1x1!");
            ASSERT(seed.cols() == 1, "Output of perceptron is 1x1!");
            b.derive(seed);
            W.derive(Nilib::transpose(x->value) * seed);
            this->partial = seed * Nilib::transpose(W.value);
            x->derive(this->partial);
        }
    };

    struct MultilayerPerceptron : public Input
    {
        CNode *x = nullptr;
        Weight W;
        Weight b;

        MultilayerPerceptron(CNode *x, size_t x_col, size_t neurons)
            : x(x),
              W(x_col, neurons),
              b(1, neurons)
        {
        }

        void evaluate() override
        {
            CORE_ASSERT(x);
            x->evaluate();
            W.evaluate();
            b.evaluate();
            ASSERT(x->value.cols() == W.value.rows(), "xW undefined", x->value.cols(), "!=", W.value.rows());
            ASSERT(x->value.rows() == b.value.rows(), "xW + b undefined input of MultilayerPerceptron should be 1xinputdim", x->value.cols(), "!=", b.value.rows());
            ASSERT(W.value.cols() == b.value.cols(), "xW + b undefined input of MultilayerPerceptron should be 1xinputdim", W.value.cols(), "!=", b.value.cols());
            this->value = x->value * W.value + b.value;
        }

        void derive(Nilib::Matrixf const &seed) override
        {
            // LOG_DEBUG("Derive multilayer perceptron", this, "with seed:", seed);
            CORE_ASSERT(x);
            ASSERT(seed.rows() == b.value.rows(), "Output of MultilayerPerceptron is 1 x outputdim!");
            ASSERT(seed.cols() == b.value.cols(), "Output of MultilayerPerceptron is 1 x outputdim!");
            b.derive(seed);

            CORE_ASSERT(x->value.rows() == seed.rows());
            auto tmp = Nilib::transpose(x->value) * seed;
            CORE_ASSERT(tmp.rows() == W.value.rows());
            CORE_ASSERT(tmp.cols() == W.value.cols());
            W.derive(tmp);

            CORE_ASSERT(seed.cols() == W.value.cols());
            this->partial = seed * Nilib::transpose(W.value);
            CORE_ASSERT(this->partial.rows() == x->value.rows());
            CORE_ASSERT(this->partial.cols() == x->value.cols());
            x->derive(this->partial);
        }
    };

    struct TwoLayerNeuralNet : public CNode
    {
        CNode *x = nullptr;
        MultilayerPerceptron mlp1;
        MultilayerPerceptron mlp2;

        TwoLayerNeuralNet(CNode *x, size_t rowX, size_t neurons1, size_t neurons2)
            : mlp1(x, rowX, neurons1),
              mlp2(&mlp2, neurons1, neurons2)
        {
        }

        void evaluate() override
        {
            LOG_DEBUG("Eval 2NN");
            CORE_ASSERT(x);
            mlp1.x = x;
            mlp2.evaluate();
            this->value = mlp2.value;
        }

        void derive(Nilib::Matrixf const &seed) override
        {
            LOG_DEBUG("Derive 2NN");
            mlp2.derive(seed);
        }
    };

    struct MLP : public Model
    {
        Input x;
        Target y;
        Param labda;
        Weight W1, W2;
        Weight b1, b2;

        DenseLayer H1, H3;
        Sigmoid H2;
        Sigmoid out;

        MSELoss MSE;
        L2 REG;
        Plus TOTLOS;

    public:
        // sigmoid(sigmoid(input * W1 + b1) * W2 + b2)
        MLP(size_t inputdim_n, size_t inputdim_m, size_t neurons1, size_t outputdim_m);
        MLP(MLP const &other);
    };
} // namespace Nilib

#endif