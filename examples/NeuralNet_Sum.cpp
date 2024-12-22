#include "ML/NeuralNet.hpp"
#include "ML/Layers/Dense.hpp"
#include "ML/Layers/Dropout.hpp"
#include "ML/Layers/Activations.hpp"

#include "Math/RNG.hpp"
#include "Core/Log.hpp"

#include <iomanip>
#include <chrono>

int main(int argc, char **argv)
{
//     if (argc != 2) {
//         Log::error("Usage: NeuralNet_Sum.exe batch_size!\n");
//         return EXIT_FAILURE;
//     }
    RNG::seed(128);
    

    NeuralNet NN;
    NN.addLayer<Dense>(2, 2);
    NN.addLayer<Relu>(1.0);
    NN.addLayer<Dense>(2, 1);
    NN.addLayer<Relu>(1.0);
    
    NN.display();
    Optimizer adam;
    adam.lr = 0.001;
    NN.compile(adam);

    size_t const batch_size = std::stoul("100");
    
    size_t iteration = 0;
    float MSE_batch_loss = std::numeric_limits<float>::max();
    
    auto start_time = std::chrono::steady_clock::now();
    while (MSE_batch_loss > 0.001)
    {
        NN.zeroGrad();

        MSE_batch_loss = 0.0;
        for (size_t batch_idx = 0; batch_idx < batch_size; ++batch_idx)
        {
            size_t sample_idx = RNG::uniform(0, 3);
            
            Matrix input = Matrixf(1, 2, {RNG::normal(0.0f, 100.0f), RNG::normal(0.0f, 100.0f)});
            Matrixf pred = NN.feedforward(input);
            Matrixf target = Matrixf(1, 1, {input.sum()});

            // Calculate the MSE loss. 
            //Matrixf loss = Matrixf(1, 1, {0.5f * ((pred(0,0) - target(0,0)) * (pred(0,0) - target(0,0)))});
            Matrixf loss = pred - target; 
            MSE_batch_loss += loss(0,0) * loss(0,0) * 0.5;
            
            /*
            Log::debug("NN prediction: ") << 
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
        Log::info("Iteration ") << iteration << " Loss: " << std::fixed << std::setprecision(5) << MSE_batch_loss << '\r';
        if (elapsed.count() > 1000) {
        Log::info("Iteration ") << iteration << " Loss: " << std::fixed << std::setprecision(5) << MSE_batch_loss << '\n';
            start_time = current_time;
        }

        ++iteration;
        NN.updateWeights();

        
    }
    
    Log::info("Iteration ") << iteration << " Loss: " << std::fixed << std::setprecision(5) << MSE_batch_loss << '\n';

    NN.display();

    for (size_t i = 0; i < 10; i++)
    {
        Matrixf input = Matrixf(1, 2, {RNG::normal(0.0f, 100.0f), RNG::normal(0.0f, 100.0f)});
        Log::info("Input: ") << input << " predict: " << NN.feedforward(input) << '|' << input.sum() << '\n';
    }
    
    
    
}