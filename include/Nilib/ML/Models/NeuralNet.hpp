#ifndef _ML_NEURALNET_HPP
#define _ML_NEURALNET_HPP

#include "Nilib/ML/Models/MultiLayerPerceptron.hpp"
#include "Nilib/ML/CNodes/Activation/Selu.hpp"
#include "Nilib/ML/CNodes/Activation/Sigmoid.hpp"

namespace Nilib
{
    /// @brief A simple neural network with two hidden layers. Takes a row layer x [1, xcol] -> hidden1 -> hidden2 -> output : [1, outputdim]
    class NeuralNet : public Module
    {
    public:
        NeuralNet(CNode &x, size_t const x_col, size_t const hidden1, size_t const hidden2, size_t const outputdim);

        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;
        void learnables(Weights &add) override;

    public:
        MultilayerPerceptron mlp1;
        Selu mlp1_act;
        MultilayerPerceptron mlp2;
        Selu mlp2_act;
        MultilayerPerceptron mlp3;
        Selu mlp3_act;
    };

} // namespace Nilib

#endif