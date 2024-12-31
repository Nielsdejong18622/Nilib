#include "Nilib/ML/CNode.h"

#include "Nilib/Math/LinAlg.hpp"

int main () {

    using namespace Nilib;
    RNG::seed(126);
    
    std::vector<Mat<1,2>> features;
    std::vector<Mat<1,1>> output;

    // Create XOR data.
    features.push_back(Mat<1,2>(1, 2, {0.0, 0.0}));
    features.push_back(Mat<1,2>(1, 2, {1.0, 0.0}));
    features.push_back(Mat<1,2>(1, 2, {0.0, 1.0}));
    features.push_back(Mat<1,2>(1, 2, {1.0, 1.0}));
    
    output.push_back(Mat<1,1>(1, 1, {0.0}));
    output.push_back(Mat<1,1>(1, 1, {1.0}));
    output.push_back(Mat<1,1>(1, 1, {1.0}));
    output.push_back(Mat<1,1>(1, 1, {0.0}));

    size_t const epochs = 250'000;

    // Weights and biases to train.
    size_t const inputdim = 2; 
    size_t const neurons1 = 2;
    size_t const neurons2 = 1;
    size_t const outputdim= 1;

    Weight<Mat<inputdim, neurons1>> W1(inputdim, neurons1);
    Weight<Mat<neurons2, neurons1>> b1(neurons2, neurons1);

    Weight<Mat<neurons1, neurons2>> W2(neurons1, neurons2);
    Weight<Mat<neurons2, outputdim>> b2(neurons2, outputdim);

    L1Loss<Mat<1,1>> weights({&W1, &b1, &W2, &b2}, 0.01);


    // For momentum updates.
    float const momentum = 0.2;
    auto uW1 = W1; uW1.value.zero();
    auto ub1 = b1; ub1.value.zero();
    
    auto uW2 = W2; uW2.value.zero();
    auto ub2 = b2; ub2.value.zero();

    float tolerance = 0.001;
    float epoch_loss = tolerance + 1.0;
    float reg_loss = 0;
    float MSE_loss = 0;

    size_t i;

    {
        PROFILE_SCOPE("Network Training");
        
        auto start_time = std::chrono::steady_clock::now();
        for (i = 0; epoch_loss > tolerance && i < epochs; ++i)
        {
            epoch_loss = reg_loss = MSE_loss = 0;

            for (size_t sample = 0; sample < features.size(); ++sample)
            {

                // Zero the gradients.
                W2.partial.zero();
                W1.partial.zero();
                b1.partial.zero();
                b2.partial.zero();

                // Input / Target output.
                Input x(features[sample]);
                Input y(output[sample]);

                // Dense Layer 1. 
                auto H1 = DenseLayer<Mat<2, 4>>(&x, &W1, &b1);
                //auto H2 = Relu(&H1, 0.01f);
                auto H2 = Tanh(&H1);

                // Dense Layer 2. 
                auto H3 = DenseLayer(&H2, &W2, &b2);
                // auto out = Relu(&H3, 0.01f);
                auto out = Tanh(&H3);
                
                //auto MSE = MSELoss(&out, &y);
                //auto REG = L1Loss(weights, 0.001);
                //auto TOTLOS = Plus(&MSE, &REG);
                /*
                // Forward.
                TOTLOS.evaluate();
                
                // Backward.
                TOTLOS.derive(Nilib::Matrixf(1, 1, {1.0f}));
                MSE_loss += MSE.value.sum() / features.size();
                reg_loss += REG.value.sum() / features.size();
                epoch_loss += (reg_loss + MSE_loss) / features.size();

                float lr = 0.05;
                // Clip the gradients so they do not explode. 
                W2.partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
                W1.partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
                b1.partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
                b2.partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
                
                // (Vanilla) Update rule. 
                // W2.value -= lr * W2.partial;
                // W1.value -= lr * W1.partial;
                // b1.value -= lr * b1.partial;
                // b2.value -= lr * b2.partial;
                

                // Momentum update rule (defaults to vanilla update when momentum = 0.0).
                uW2 = momentum * uW2 + (lr / features.size()) * W2.partial;
                uW1 = momentum * uW1 + (lr / features.size()) * W1.partial;
                ub2 = momentum * ub2 + (lr / features.size()) * b2.partial;
                ub1 = momentum * ub1 + (lr / features.size()) * b1.partial;
                
                W2.value -= uW2;
                W1.value -= uW1;
                b1.value -= ub1;
                b2.value -= ub2;
                */
            }
            
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
            if (elapsed.count() > 100) {
                LOG_PROGRESS("Epoch", i, "MSE loss:", MSE_loss, "Reg loss:", reg_loss, "total loss:", epoch_loss);
                start_time = current_time;
            }
        }
    } // Profile scope. 
    
    /*
    
    
    LOG_DEBUG("Network Final Weights:");
    for (auto &&weight : weights)
        LOG_DEBUG(*weight);
    // After training, do some prediction. 
    for (size_t sample = 0; sample < features.size(); ++sample)
    {
        Input x(features[sample]);
        Input y(output[sample]);

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
        
        LOG_INFO("MLP(", features[sample](0,0), features[sample](0,1), ")=", std::fixed, std::setprecision(4), out.value(0,0), "target", output[sample](0,0));
    }
    
    */
    return 0;
}