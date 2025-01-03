#include "Nilib/ML/MLData.h"


namespace Nilib {

    // Create XOR classification dataset.
    MLData MLData::XOR() 
    {
        std::vector<Matrixf> features, output;
        features.reserve(4 * 3); output.reserve(4 * 3);
        // Training data, validation data and test data are all the same. 
        for (size_t rep = 0; rep < 3; ++rep)
        {
            features.push_back(Matrixf(1, 2, {0.0, 0.0}));
            features.push_back(Matrixf(1, 2, {1.0, 0.0}));
            features.push_back(Matrixf(1, 2, {0.0, 1.0}));
            features.push_back(Matrixf(1, 2, {1.0, 1.0}));
            output.push_back(Matrixf(1, 1, {0.0}));
            output.push_back(Matrixf(1, 1, {1.0}));
            output.push_back(Matrixf(1, 1, {1.0}));
            output.push_back(Matrixf(1, 1, {0.0}));
        }

        return MLData{features, output, 4, 8};
    }

    // A classiciation dataset with uniform sampled x and y coordinates and 
    // label 1 if x^2 + y^2 < 1. 70% 20% 10% split.
    MLData MLData::CIRCLE(size_t ntrain_points)
    {
        std::vector<Matrixf> features, output;
        size_t nobs = 100.0f * ntrain_points / 70.0f;
        features.reserve(nobs); output.reserve(nobs);
        // Training data, validation data and test data are all the same. 
        for (size_t rep = 0; rep < nobs; ++rep)
        {
            float x = RNG::uniform(-1.0, 1.0);
            float y = RNG::uniform(-1.0, 1.0);
            float label = (x * x + y * y < 1.0f) ? 1.0f : 0.0f;
            features.push_back(Matrixf(1, 2, {x, y}));
            output.push_back(Matrixf(1, 1, {label}));
        }
        size_t n_test = 0.9 * nobs;
        return MLData{features, output, ntrain_points, n_test};
    }


    void MLData::shuffle(size_t low, size_t high)
    {
        // Length of the subarray to shuffle.
        size_t nobs = high - low;
        // Shuffle in place. (Fisher-Yates shuffle)
        for (size_t idx = low; idx < nobs - 1; ++idx)
        {
            size_t jdx = RNG::index(nobs) + low;
            std::swap(X[idx], X[jdx]);
            std::swap(Y[idx], Y[jdx]);
        }
    }

    void MLGraphData::shuffle(size_t low, size_t high)
    {
        // Length of the subarray to shuffle.
        size_t nobs = high - low;
        // Shuffle in place. (Fisher-Yates shuffle)
        for (size_t idx = low; idx < nobs - 1; ++idx)
        {
            size_t jdx = RNG::index(nobs) + low;
            std::swap(X[idx], X[jdx]);
            std::swap(Y[idx], Y[jdx]);
            std::swap(A[idx], A[jdx]);
        }
    }
}