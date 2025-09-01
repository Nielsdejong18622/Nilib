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
        // Elements [0, training_end) are for training.
        size_t training_end = 0;
        // Elements [training_end, validation_end) are for validation.
        size_t validation_end = 0;
        // Elements [validation_end, obs()) are for testing.

        size_t obs() { return X.size(); };
        size_t train_obs() { return training_end; }
        size_t valid_obs() { return validation_end - training_end; }
        size_t test_obs() { return obs() - validation_end; }

        // Add some noise to the dataset.
        // void addYNoise(float p);
        // void addXNoise(float p);

        // Shuffle the observations
        void shuffle() { shuffle(0, obs()); };
        // Shuffle the observations between [low and high].
        void shuffle(size_t low, size_t high);

        // The XOR classification dataset.
        static MLData XOR();

        static MLData CIRCLE(size_t ntrain_points = 100);

        // Dimensions of the data.
        size_t Xrows() const { return X.front().rows(); }
        size_t Xcols() const { return X.front().cols(); }
        size_t Yrows() const { return Y.front().rows(); }
        size_t Ycols() const { return Y.front().cols(); }

        // Save and Load.
    };

    // A ML supervised graph dataset.
    struct MLGraphData : public MLData
    {
        std::vector<Matrixf> A;

        void shuffle(size_t low, size_t hgih);
    };

} // namespace Nilib

#endif