#include "Nilib/ML/Models/NeuralNet.hpp"

using namespace Nilib;

NeuralNet::NeuralNet(CNode &x, size_t const x_col, size_t const hidden1, size_t const hidden2, size_t const outputdim)
    : mlp1(x, x_col, hidden1),
      mlp1_act(mlp1),
      mlp2(mlp1_act, hidden1, hidden2),
      mlp2_act(mlp2),
      mlp3(mlp2_act, hidden2, outputdim),
      mlp3_act(mlp3)
{
}

void NeuralNet::evaluate()
{
    mlp3_act.evaluate();
    this->value = mlp3_act.value;
}
void NeuralNet::derive(Nilib::Matrixf const &seed)
{
    mlp3_act.derive(seed);
}
void NeuralNet::learnables(Weights &add)
{
    mlp1.learnables(add);
    mlp2.learnables(add);
    mlp3.learnables(add);
}
