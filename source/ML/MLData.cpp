#include "Nilib/ML/MLData.h"


namespace Nilib {

    // Create XOR data.
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