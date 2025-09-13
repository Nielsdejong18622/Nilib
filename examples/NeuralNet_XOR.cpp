#include "Nilib/Math/RNG.hpp"

#include "Nilib/Logger/Log.hpp"
#include "Nilib/ML/ML.hpp"

int main(int argc, char **argv)
{
    using namespace Nilib;
    Nilib::RNG::seed(126);

    // // Set up the data to learn from.
    MLData<XORobs> data_xor = XOR();

    // // Define the Computational Graph.
    Input x;
    Target y;
    NeuralNet nn(x, XORobs::Xcol, 10, 10, XORobs::Ycol);

    // Define the loss.
    MSELoss mse(nn, y);

    // Obtain the learnable weights.
    Adam optimizer(&nn, 0.01f);

    LOG_INFO("Now fitting Neural Net on XOR!");
    train(data_xor, 1000, x, y, mse, optimizer);

    LOG_INFO("Now fitting Neural Net on circle!");
    MLData<CIRCLEobs> data_circle = CIRCLE();
    train(data_circle, 1'000'000, x, y, mse, optimizer, 1000);

    LOG_DEBUG("Network Final Weights:");
    optimizer.printWeights();
}