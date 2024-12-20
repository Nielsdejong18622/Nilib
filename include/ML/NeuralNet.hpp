#ifndef _NEURAL_NET_H
#define _NEURAL_NET_H

#include "Math/Matrix.hpp"
#include "ML/Layers/NeuralLayer.hpp"
#include "ML/Optimizer.hpp"
#include <vector>




// A NeuralNet is a set of neural layers in a computational graph.
// It is a function.  
class NeuralNet
{
protected:
    Optimizer d_optimizer;
    std::vector<NeuralLayer*> d_layers;

    Matrixf d_adjacency;
public:

    //NeuralNet();

    // Display the layout of the computational graph. 
    void display() const;
    void info() const;

    // Output some weight characteristics (usefull for training).
    void monitorWeights() const;

    // Add a layer to the NeuralNet. 
    template <typename Layer, typename... cArgs>
    void addLayer(cArgs&&... args) {
        d_layers.emplace_back(new Layer(std::forward<cArgs>(args)...));
        d_layers.back()->d_model = this;
    }

    // Make a prediction based on some input features X.  
    Matrixf feedforward(Matrixf const &X);

    // Calculates and accumulates gradient in all applicable neurallayers.  
    Matrixf backward(Matrixf const &loss);

    // Add an optimizer and loss function. 
    void compile(Optimizer const &optimizer);

    // Update weights using the selected optimizer and accumulated gradient.  
    void updateWeights();

    // Visit every layer and reset the gradient. 
    void zeroGrad();

    // Get and set the adjacency matrix. 
    Matrixf const &adjacency() const { return d_adjacency; };
    void adjacency(Matrixf const &A) { d_adjacency = A; }
};


#endif