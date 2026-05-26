#include "Nilib/Math/RNG.hpp"

#include "Nilib/Logger/Log.hpp"
#include "Nilib/ML/ML.hpp"

int main()
{
    using namespace Nilib;
    Nilib::RNG::seed(126);

    // // Set up the data to learn from.
    MLData<XORobs> data_xor = XOR();

    // // Define the Computational Graph.
    Input x;
    Target y;
    NeuralNet<Nilib::Selu> nn(x, XORobs::Xcol, 10, 10, XORobs::Ycol);

    // Define the loss.
    MSELoss mse(nn, y);

    // Obtain the learnable weights.
    Adam optimizer(&nn, 0.001f);

    LOG_INFO("Now fitting Neural Net on XOR!");
    train(data_xor, 1000, x, y, mse, optimizer);

    MLData<CIRCLEobs> data_circle = CIRCLE();
    LOG_INFO("Now fitting Neural Net on circle!");
    train(data_circle, 1000, x, y, mse, optimizer);

    LOG_DEBUG("Network Final Weights:");
    optimizer.printWeights();
}