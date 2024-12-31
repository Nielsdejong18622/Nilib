#include "Nilib/ML/Model.h"
#include "Nilib/Logger/Log.hpp"

namespace Nilib {

    Model::Model(Inputs const &inputs, Weights const &weights, CNode *prediction, Target *target, CNode *loss)
    : inputs(inputs), weights(weights), prediction(prediction), target(target), loss(loss)
    {
    }
    // Model::Model(std::initializer_list<Input*> const &inputs, std::initializer_list<Weight*> const &weights, CNode *prediction, Target *target, CNode *loss)
    // : inputs(inputs), weights(weights), prediction(prediction), target(target), loss(loss)
    // {
    // }

    void Model::updateGrad()        
    {
        float const lr = 0.02;
        for (auto &&weight : weights)
        {
            weight->value -= lr * weight->partial;
        }
    }

    void Model::zeroGrad()
    {
        for (auto &&weight : weights)
        {
            weight->partial.zero();
        }
    }

    void Model::train(MLData const &data, size_t numepochs)
    {
        Matrixf const seed = Nilib::Matrixf(1, 1, {1.0f});
        auto start_time = std::chrono::steady_clock::now();

        size_t epoch;
        for (epoch = 0; epoch < numepochs; ++epoch)
        {
            for (size_t sample = 0; sample < data.training_end; ++sample)
            {
                // Zero the gradients.
                zeroGrad();

                // Set the inputs.
                inputs[0]->set(data.X[sample]);
                //inputs[1]->set(data.A[sample]);
                
                // Set the target. 
                target->set(data.Y[sample]);
                
                // Forward.
                loss->evaluate();
                
                // Backward.
                loss->derive(seed);
                
                // Clip the gradients so they do not explode. 
                // W2.partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
                // W1.partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
                // b1.partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
                // b2.partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
                
                // (Vanilla) Update rule. 
                updateGrad();
            }
            
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
            if (elapsed.count() > 100) {
                LOG_PROGRESS("Epoch", epoch);
                start_time = current_time;
            }
        }
    }

}