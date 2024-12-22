#include "Nilib/ML/NeuralNet.hpp"



void NeuralNet::display() const
{
    for (auto &&layer : d_layers)
    {
        layer->display();
    }
    
}
void NeuralNet::info() const
{
    for (auto &&layer : d_layers)
    {
        layer->info();
    }
    
}
// Output some weight characteristics (usefull for training).
void NeuralNet::monitorWeights() const
{

}

// Make a prediction based on some input features X.  
Matrixf NeuralNet::feedforward(Matrixf const &X)
{
    //Log::info("Feedforward adjacency: ") << d_adjacency << '\n';
    Matrixf X1 = X;
    for (auto &&layer : d_layers)
    {
        //layer->display();
        //Log::info("Feedforward input: ") << X1 << '\n';
        X1 = layer->forward(X1);
        //Log::info("Feedforward output: ") << X1 << '\n';
    }
    return X1;
}

// Calculates and accumulates gradient in all applicable neurallayers.  
Matrixf NeuralNet::backward(Matrixf const &Loss)
{
    Matrixf X1 = Loss;
    //if (X == Matrixf::all(X.rows(), X.cols(), 0.0)) return X;
    for (int lay = d_layers.size() - 1; lay >= 0; --lay)
    {
        //d_layers[lay]->display();
        //Log::debug("Backward input: ") << X1 << '\n';
        //if (X1 == Matrixf::all(X1.rows(), X1.cols(), 0.0)) break;
        X1 = d_layers[lay]->backward(X1);
        //Log::debug("Backward output: ") << X1 << '\n';
    }
    return X1;
}

// Add an optimizer and loss function. 
void NeuralNet::compile(Optimizer const &optimizer)
{
    d_optimizer = optimizer;
    zeroGrad();
}

// Update weights using the selected optimizer and accumulated gradient.  
void NeuralNet::updateWeights()
{
    for (auto &layer : d_layers)
        layer->update(d_optimizer);
}

// Visit every layer and reset the gradient. 
void NeuralNet::zeroGrad()
{
    for (NeuralLayer *layer : d_layers)
        layer->zeroGrad();
}

