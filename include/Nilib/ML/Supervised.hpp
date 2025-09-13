#include "Nilib/ML/MLData.hpp"

#include "Nilib/ML/CNodes/Input.hpp"

namespace Nilib
{
    template <typename MLData>
    void train(MLData &data, size_t const max_epochs, Input &x, Target &y, CNode &module, Optimizer &optimizer, size_t batch_size = 10, size_t log_every = 10)
    {
        for (size_t train_epoch = 0; train_epoch < max_epochs; ++train_epoch)
        {
            RunningStats train_loss, valid_loss, batch;

            // Pass through the dataset.
            data.shuffle_training();
            for (auto &obs : data.train())
            {
                x.set(obs.X);
                y.set(obs.Y);

                module.evaluate();
                train_loss.push(module.value(0));
                module.derive({1.0f});

                if (train_loss.n() % batch_size == 0)
                {
                    // Update the weight.
                    optimizer.updateGrad(1.0f);
                    optimizer.zeroGrad();
                }
            }

            // Update the weight.
            optimizer.updateGrad(1.0f);
            optimizer.zeroGrad();
            for (auto &obs : data.validate())
            {
                x.set(obs.X);
                y.set(obs.Y);
                module.evaluate();
                valid_loss.push(module.value(0));
            }

            if (Signal::receivedCtrlC())
                break;
            // Log progress (to file)
            if (train_epoch == 0 || (train_epoch + 1) % log_every == 0)
                LOG_INFO("Epoch:", train_epoch + 1, "Train loss:", train_loss.mean(), "Validation loss:", valid_loss.mean());
        }

        // Report final test_loss;

        RunningStats test_loss;
        for (auto &obs : data.test())
        {
            x.set(obs.X);
            y.set(obs.Y);

            module.evaluate();
            test_loss.push(module.value(0));
        }
        LOG_INFO("Finished training, Test loss:", test_loss.mean());
    }

}