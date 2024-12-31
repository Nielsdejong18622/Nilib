#ifndef _ML_DATA_H
#define _ML_DATA_H

#include <vector>
#include "Nilib/Math/Matrix.hpp"

namespace Nilib
{
    // A ML supervised dataset.
    struct MLData
    {
        
        std::vector<Matrixf> X;
        std::vector<Matrixf> Y;
        size_t training_obs = 0;
        size_t validation_obs = 0;
        size_t test_obs = 0;
        size_t obs() { return training_obs + validation_obs + test_obs; };

        // Add some noise to the dataset.
        // void addYNoise(float p);
        // void addXNoise(float p);

        // Shuffle the observations
        void shuffle();

        // The XOR classification dataset.
        static MLData XOR();

        // Save and Load.
    };

    // A ML supervised graph dataset.
    struct MLGraphData : public MLData
    {
        std::vector<Matrixf> A;

        void shuffle();
    };
    
} // namespace Nilib

#endif