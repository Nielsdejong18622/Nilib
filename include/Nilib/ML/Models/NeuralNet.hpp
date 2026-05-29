#ifndef _ML_NEURALNET_HPP
#define _ML_NEURALNET_HPP

#include "Nilib/ML/Models/MultiLayerPerceptron.hpp"
#include "Nilib/ML/CNodes/Activation/Linear.hpp"
#include "Nilib/ML/CNodes/Activation/Selu.hpp"
#include "Nilib/ML/CNodes/Activation/Relu.hpp"
#include "Nilib/ML/CNodes/Activation/Sigmoid.hpp"
#include "Nilib/ML/CNodes/Activation/Tanh.hpp"

namespace Nilib
{
    template <typename InnerLayer = LeakyRelu, typename OutputLayer = Relu>
    class NeuralNet : public Module
    {
    public:
        NeuralNet(CNode &x, size_t const x_col, size_t const hidden1, size_t const hidden2, size_t const outputdim);

        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;
        void learnables(Weights &add) override;

    public:
        MultilayerPerceptron mlp1;
        InnerLayer mlp1_act;
        MultilayerPerceptron mlp2;
        InnerLayer mlp2_act;
        MultilayerPerceptron mlp3;
        OutputLayer mlp3_act;
    };

    template <typename InnerLayer, typename OutputLayer>
    NeuralNet<InnerLayer, OutputLayer>::NeuralNet(CNode &x, size_t const x_col, size_t const hidden1, size_t const hidden2, size_t const outputdim)
        : mlp1(x, x_col, hidden1),
          mlp1_act(mlp1),
          mlp2(mlp1_act, hidden1, hidden2),
          mlp2_act(mlp2),
          mlp3(mlp2_act, hidden2, outputdim),
          mlp3_act(mlp3)
    {
    }

    template <typename InnerLayer, typename OutputLayer>
    void NeuralNet<InnerLayer, OutputLayer>::evaluate()
    {
        mlp3_act.evaluate();
        this->value = mlp3_act.value;
    }
    template <typename InnerLayer, typename OutputLayer>
    void NeuralNet<InnerLayer, OutputLayer>::derive(Nilib::Matrixf const &seed)
    {
        mlp3_act.derive(seed);
    }

    template <typename InnerLayer, typename OutputLayer>
    void NeuralNet<InnerLayer, OutputLayer>::learnables(Weights &add)
    {
        mlp1.learnables(add);
        mlp2.learnables(add);
        mlp3.learnables(add);
    }

} // namespace Nilib

#endif