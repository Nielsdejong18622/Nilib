#include "Nilib/ML/ML.h"
#include "Nilib/ML/MLData.h"

int main()
{
    Nilib::RNG::seed(126);

    // using namespace Nilib;

    // // Set up the data to learn from.
    // MLData data = MLData::CIRCLE();

    // // Define the Computational Graph.

    // MLP model(1, 2, 5, 1);
    // MLP model_start(model);

    // // Make the model fit the data in 250'000 epochs.
    // model.train(data, 25000, 4);

    // LOG_DEBUG("Network Final Weights:");
    // for (auto &&weight : model.weights)
    //     LOG_DEBUG(*weight);

    // for (size_t sample = data.test_obs(); sample < data.obs(); ++sample)
    // {
    //     auto [x, y] = std::tie(data.X[sample], data.Y[sample]);
    //     auto pred = model.predict(x);
    //     auto pred_s = model_start.predict(x);
    //     LOG_INFO("Model2 Input:", x(0, 0), x(0, 1), "Pred:", pred(0, 0), "Pred_start", pred_s(0, 0), "target", y(0, 0));
    // }

    return 0;
}