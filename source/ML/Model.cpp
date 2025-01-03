#include "Nilib/ML/Model.h"
#include "Nilib/Logger/Log.hpp"
#include "Nilib/ML/MLData.h"
#include "Nilib/Core/Profiler.hpp"
#include "Nilib/Math/Stats.hpp"

using namespace Nilib;

Model::Model(Inputs const &inputs, Weights const &weights, CNode *prediction, Target *target, CNode *loss)
: inputs(inputs), weights(weights), prediction(prediction), target(target), loss(loss)
{
}
// Model::Model(std::initializer_list<Input*> const &inputs, std::initializer_list<Weight*> const &weights, CNode *prediction, Target *target, CNode *loss)
// : inputs(inputs), weights(weights), prediction(prediction), target(target), loss(loss)
// {
// }

Matrixf &Model::predict(Matrixf const &x)
{
    inputs[0]->set(x);

    prediction->evaluate();

    return prediction->value;
}

void Model::updateGrad()        
{
    float const lr = 0.02;
    for (auto &&weight : weights)
    {
        // Clamp the weight so they do not explode.
        weight->partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
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

    PROFILE_SCOPE("Model training.");

    RunningStats train, batch, validation;

    size_t epoch;
    for (epoch = 0; epoch < numepochs; ++epoch)
    {
        batch.reset(); validation.reset();
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
                        
            // (Vanilla) Update rule. 
            updateGrad();

            // Logging. 
            batch.push(loss->value.sum());
        }

        // Validation.
        for (size_t sample = data.training_end; sample < data.validation_end; ++sample)
        {
            // Set the inputs.
            inputs[0]->set(data.X[sample]);
            //inputs[1]->set(data.A[sample]);
            
            // Set the target. 
            target->set(data.Y[sample]);
            
            // Forward.
            loss->evaluate();
            
            // Logging. 
            validation.push(loss->value.sum());
        }
        
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
        if (elapsed.count() > 100 || epoch == numepochs - 1) {
            LOG_PROGRESS() << "Epoch " 
                           << epoch + 1 
                           << " loss " 
                           << batch.mean() 
                           //<< '(' 
                           //<< batch.variance()
                           //<< ')' 
                           << " validation "
                           << validation.mean() 
                           << "\n";
            start_time = current_time;
        }
    }
    
}