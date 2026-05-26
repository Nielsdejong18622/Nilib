#include "Nilib/Logger/Log.hpp"
#include "Nilib/Logger/CSV.hpp"
#include "Nilib/ML/ML.hpp"

using namespace Nilib;

float f(float t);
float f(float t)
{
    // return std::sin(t);
    // return 1.0 - std::exp(-t);
    return 1.0f / (1.0f + std::exp(-t));
}

void train_pinn(CSVWriter &writer, float labda = 0.0);
void train_pinn(CSVWriter &writer, float labda)
{
    Nilib::RNG::seed(127);
    float a = -2.5f;
    float b = 2.5f;
    size_t data_size = 20;
    std::vector<float> data(data_size);
    std::vector<float> vdata(data_size * 4);
    for (size_t i = 0; i < data_size; i++)
        data[i] = RNG::normal(0.5 * (a + b), 1.0);
    for (size_t i = 0; i < data_size * 4; i++)
        vdata[i] = RNG::uniform(a * 10.0, b * 10.0);

    Input t;
    NeuralNet<Selu, Selu> NN(t, 1, 8, 8, 1);
    Nilib::Adam optimizer(NN, 0.01);

    LOG_DEBUG("Data:", data);
    LOG_DEBUG("VData:", vdata);

    for (size_t train_epoch = 0; train_epoch < 10'000; ++train_epoch)
    {
        RunningStats train_loss, train_p_loss, valid_loss;

        // Pass through the dataset. Sample loss.
        // RNG::shuffle(data);
        for (auto rt : data)
        {
            optimizer.zeroGrad();
            t.set({rt});

            NN.evaluate();
            float nn = NN.value(0);
            float loss = nn - f(rt);

            float physicsloss = 0.0;
            if (labda != 0.0)
            {
                t.partial.zero();
                NN.derive({1.0});
                float f_t = t.partial(0);
                optimizer.zeroGrad();
                physicsloss = labda * (f_t - nn * (1.0 - nn)) * (-1.0 + 2.0 * nn); // For the sigmoid.
                // physicsloss = labda * (f_t + nn);                                  // For the sin_wave
            }

            train_loss.push(std::abs(loss));
            train_p_loss.push(std::abs(physicsloss));
            NN.derive({loss + physicsloss});
            optimizer.updateGrad();
        }

        // Validation loss.
        for (auto rt : vdata)
        {
            t.set({rt});

            NN.evaluate();
            float loss = NN.value(0) - f(rt);

            valid_loss.push(std::abs(loss));
        }

        if (Signal::receivedCtrlC())
            break;
        // Log progress (to file)
        if (train_loss.mean() + train_p_loss.mean() < 0.005)
            break;
        if (train_epoch == 0 || (train_epoch + 1) % 100 == 0)
        {
            LOG_INFO("Epoch:", train_epoch + 1, "Train loss:", train_loss.mean(), "Train Phys loss:", train_p_loss.mean(), "Validation loss:", valid_loss.mean());

            train_loss.reset();
            train_p_loss.reset();
            valid_loss.reset();
        }
    }

    // Report final test_loss;
    RunningStats test_loss;
    for (long int b_idx = a * 10.0 * 1000; b_idx < b * 10.0 * 1000; ++b_idx)
    {
        float rt = b_idx / 1000.0;
        t.set({rt});
        NN.evaluate();

        test_loss.push(std::sqrt((NN.value(0) - f(rt)) * (NN.value(0) - f(rt))));
        writer.write_row(rt, NN.value(0), f(rt), labda);
    }
    LOG_INFO("Finished training, Test loss:", test_loss.mean());
}

int main()
{
    CSVWriter writer("PINN.csv");
    writer.write_header("time", "NN", "true", "labda");
    train_pinn(writer, 0.0f);   // NO PINN
    train_pinn(writer, 0.001f); // NO PINN
    train_pinn(writer, 0.1f);   // PINN!
    train_pinn(writer, 0.2f);   // PINN!
    train_pinn(writer, 1.0f);   // PINN!
}