#include "Nilib/Logger/Log.hpp"
#include "Nilib/ML/ML.hpp"

float f(float t);
float f(float t)
{
    return 1.0f / (1.0f + std::exp(-t));
}

using namespace Nilib;

int main()
{
    Input t;
    Target y;
    NeuralNet<Selu> NN(t, 1, 10, 10, 1);
    MSELoss loss(NN, y);
    Nilib::Adam optimizer(NN);
    Nilib::RNG::seed(127);

    size_t batch_size = 10;
    size_t data_size = 10;
    for (size_t train_epoch = 0; train_epoch < 1'000; ++train_epoch)
    {
        RunningStats train_loss, train_p_loss, valid_loss, batch;

        // Pass through the dataset.
        for (size_t b_idx = 0; b_idx < data_size; ++b_idx)
        {
            float rt = RNG::uniform(0.0f, 10.0f);
            t.set({rt});
            y.set({f(rt)});

            NN.evaluate();

            float loss = NN.value(0) - y.value(0);

            optimizer.zeroGrad();
            t.partial.zero();
            NN.derive({1.0});
            // LOG_DEBUG(t.partial(0));
            float physicsloss = t.partial(0) - f(rt) * (1 - f(rt));
            // float physicsloss = 0.0f;
            train_p_loss.push(physicsloss);
            train_loss.push(loss + physicsloss);
            
            optimizer.zeroGrad();
            NN.derive({loss});

            optimizer.updateGrad();
        }

        // Update the weight.
        for (size_t b_idx = 0; b_idx < data_size; ++b_idx)
        {
            float rt = RNG::uniform(0.0f, 10.0f);
            t.set({rt});
            y.set({f(rt)});

            NN.evaluate();
            float loss = NN.value(0) - y.value(0);

            valid_loss.push(loss);
        }

        if (Signal::receivedCtrlC())
            break;
        // Log progress (to file)
        if (train_epoch == 0 || (train_epoch + 1) % 10 == 0)
            LOG_INFO("Epoch:", train_epoch + 1, "Train loss:", train_loss.mean(), "Train Phys loss:", train_p_loss.mean(), "Validation loss:", valid_loss.mean());
    }

    // Report final test_loss;

    RunningStats test_loss;
    for (size_t b_idx = 0; b_idx < data_size; ++b_idx)
    {
        float rt = RNG::uniform(0.0f, 10.0f);
        t.set({rt});
        y.set({f(rt)});

        NN.evaluate();
        float loss = NN.value(0) - y.value(0);

        test_loss.push(loss);
    }
    LOG_INFO("Finished training, Test loss:", test_loss.mean());
}
