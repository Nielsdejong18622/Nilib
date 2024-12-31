#include "Nilib/ML/ML.h"

#include "Nilib/Core/Profiler.hpp"

int main () {

    Nilib::RNG::seed(126);
    
    using namespace Nilib;


    // Set up the data to learn from.
    MLData data = MLData::XOR();


    // Define the Computational Graph. 
    size_t const nnodes = 1;
    size_t const inputdim = 2;
    size_t const neurons1 = 20;
    size_t const neurons2 = 1;
    size_t const outputdim= 1;

    Input x;
    //Input A; // (Matrixf(1,1,{1.0f}));
    Target y;

    Param labda(0.01f);

    Weight W1(inputdim, neurons1);
    Weight b1(neurons2, neurons1);
    Weight W2(neurons1, neurons2);
    Weight b2(neurons2, outputdim);
    
    auto H1 = DenseLayer(&x, &W1, &b1);
    auto H2 = Relu(&H1, 0.01f);
    //auto H2 = Tanh(&H1);
    
    auto H3 = DenseLayer(&H2, &W2, &b2);
    auto out = Tanh(&H3);
    
    auto MSE = MSELoss(&out, &y);
    auto REG = L2({&W1, &b1, &W2, &b2}, &labda);
    auto TOTLOS = Plus(&MSE, &REG);
    
    // Setup the model on the computational graph. 
    Model model({&x}, {&W1, &W2, &b1, &b2}, &out, &y, &TOTLOS);


    LOG_DEBUG("Network Starting Weights:");
    for (auto &&weight : model.weights)
        LOG_DEBUG(*weight);

    // Make the model fit the data in 250'000 epochs. 
    model.train(data, 250'000);

    // Set the Optimizer.
    float const momentum = 0.2;
    float lr = 0.05;

    // Track Convergence statistics.
    float tolerance = 0.001;
    float epoch_loss = tolerance + 1.0;
    float reg_loss = 0;
    float MSE_loss = 0;

    size_t i;


    return EXIT_SUCCESS;

    
    
    
    LOG_DEBUG("Network Final Weights:");
    for (auto &&weight : {&W1, &b1, &W2, &b2} )
        LOG_DEBUG(*weight);
    // After training, do some prediction on the test dataset. 
    for (size_t sample = data.validation_end; sample < data.obs(); ++sample)
    {
        Input x(data.X[sample]);
        Input y(data.Y[sample]);

        // Dense Layer 1. 
        auto H1 = DenseLayer(&x, &W1, &b1);
        //auto H2 = Relu(&H1, 0.01f);
        auto H2 = Tanh(&H1);

        // Dense Layer 2. 
        auto H3 = DenseLayer(&H2, &W2, &b2);
        // auto out = Relu(&H3, 0.01f);
        auto out = Tanh(&H3);
        
        // Forward.
        out.evaluate();
        
        float x0 = data.X[sample](0,0);
        float x1 = data.X[sample](0,1);
        float target = data.Y[sample](0,0);
        float pred = out.value(0,0);
        LOG_INFO("MLP(", x0, x1, ")=", std::fixed, std::setprecision(4), pred, "target", target);
    }
    
    return 0;
}