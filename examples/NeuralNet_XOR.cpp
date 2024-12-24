#include "Nilib/ML/NeuralNet.hpp"
#include "Nilib/ML/Layers/Dense.hpp"
#include "Nilib/ML/Layers/Dropout.hpp"
#include "Nilib/ML/Layers/Activations.hpp"

#include "Nilib/Math/RNG.hpp"
#include "Nilib/Logger/Log.hpp"

#include <iomanip>
#include <chrono>

using namespace Nilib;

int main(int argc, char **argv)
{
    if (argc != 3) {
        LOG_ERROR() << "Usage: NeuralNet_XOR.exe max_iter batch_size!\n";
        return EXIT_FAILURE;
    }
    RNG::seed(127);
    // Create XOR data.
    std::vector<Matrixf> features;
    std::vector<Matrixf> output;
    
    features.push_back(Matrixf(1, 2, {0.0, 0.0}));
    features.push_back(Matrixf(1, 2, {1.0, 0.0}));
    features.push_back(Matrixf(1, 2, {0.0, 1.0}));
    features.push_back(Matrixf(1, 2, {1.0, 1.0}));
    
    output.push_back(Matrixf(1, 1, {0.0}));
    output.push_back(Matrixf(1, 1, {1.0}));
    output.push_back(Matrixf(1, 1, {1.0}));
    output.push_back(Matrixf(1, 1, {0.0}));

    for (size_t i = 0; i < 4; i++)
    {
        features[i] *= 10000;
        LOG_DEBUG() << "Features " << features[i] << '\n';
    }
    

    NeuralNet NN;
    NN.addLayer<Dense>(2, 200);
    NN.addLayer<Sigmoid>();
    NN.addLayer<Dense>(200, 1);
    NN.addLayer<Sigmoid>();
    NN.display();
    Optimizer adam;
    adam.lr = 0.01;
    NN.compile(adam);

    size_t const max_iterations = std::stoul(argv[1]);
    size_t const batch_size = std::stoul(argv[2]);
    
    size_t iteration = 0;
    float MSE_batch_loss = std::numeric_limits<float>::max();
    float avg_MSE_batch_loss = 10;



    auto start_time = std::chrono::steady_clock::now();
    while (avg_MSE_batch_loss > 0.001)
    {
        NN.zeroGrad();

        avg_MSE_batch_loss = 0.1 * MSE_batch_loss + (1 - 0.1) * avg_MSE_batch_loss;
        MSE_batch_loss = 0.0;
        for (size_t batch_idx = 0; batch_idx < batch_size; ++batch_idx)
        {
            size_t sample_idx = RNG::index(3);
            
            Matrixf pred = NN.feedforward(features[sample_idx]);
            Matrixf target = output[sample_idx];

            // Calculate the MSE loss. 
            //Matrixf loss = Matrixf(1, 1, {0.5f * ((pred(0,0) - target(0,0)) * (pred(0,0) - target(0,0)))});
            Matrixf loss = pred - target; 
            MSE_batch_loss += loss(0,0) * loss(0,0) * 0.5;
            
            /*
            LOG_DEBUG() << "NN prediction: " << 
                std::fixed << std::setprecision(5) << pred(0,0) << 
                " (" << features[sample_idx](0,0) << ',' << features[sample_idx](0,1) << ")=" << 
                target(0,0) << "\tLoss:" << loss(0,0) * loss(0,0) << '\n'; 
            */
            
            
            // Accumulate the gradient backwards. 
            NN.backward(loss);
        }
        MSE_batch_loss /= batch_size;


        auto current_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
        if (elapsed.count() > 100) {
            LOG_INFO() << "Iteration " << iteration << " Loss: " << std::fixed << std::setprecision(5) << avg_MSE_batch_loss << '|' << MSE_batch_loss << '\n';
            start_time = current_time;
        }

        ++iteration;
        NN.updateWeights();

        
    }
    
    LOG_INFO() << "Iteration " << iteration << " Loss: " << std::fixed << std::setprecision(5) << avg_MSE_batch_loss << '\n';

    NN.display();

    for (size_t i = 0; i < 4; i++)
    {
        LOG_INFO() << "Input: " << features[i] << " predict: " << NN.feedforward(features[i]) << '|' << output[i] << '\n';
    }
    
    
    
}