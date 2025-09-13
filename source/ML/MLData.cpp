#include "Nilib/ML/MLData.hpp"

namespace Nilib
{

    // Create XOR classification dataset.
    MLData<XORobs> XOR()
    {
        std::vector<XORobs> data;
        data.reserve(4 * 3);
        // Training data, validation data and test data are all the same.
        for (size_t rep = 0; rep < 3; ++rep)
        {
            data.emplace_back(XORobs{.X = Matrixf({0.0, 0.0}), .Y = Matrixf({0.0})});
            data.emplace_back(XORobs{.X = Matrixf({1.0, 0.0}), .Y = Matrixf({1.0})});
            data.emplace_back(XORobs{.X = Matrixf({0.0, 1.0}), .Y = Matrixf({1.0})});
            data.emplace_back(XORobs{.X = Matrixf({1.0, 1.0}), .Y = Matrixf({0.0})});
        }

        return MLData<XORobs>(data, 4, 8);
    }

    // A classiciation dataset with uniform sampled x and y coordinates and
    // label 1 if x^2 + y^2 < 1. 70% 20% 10% split.
    MLData<CIRCLEobs> CIRCLE(size_t const ntrain_points)
    {
        std::vector<CIRCLEobs> data;
        size_t nobs = 100.0f * ntrain_points / 70.0f;
        data.reserve(nobs);

        // Training data, validation data and test data are all the same.
        for (size_t rep = 0; rep < nobs; ++rep)
        {
            float x = RNG::uniform(-1.0, 1.0);
            float y = RNG::uniform(-1.0, 1.0);
            float label = (x * x + y * y < 1.0f) ? 1.0f : 0.0f;
            data.emplace_back(CIRCLEobs{.X = Matrixf({x, y}), .Y = Matrixf({label})});
        }
        size_t n_test = 0.9 * nobs;
        return MLData<CIRCLEobs>{data, ntrain_points, n_test};
    }

}