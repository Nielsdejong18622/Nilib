#ifndef _ML_DATA_H
#define _ML_DATA_H

#include "Nilib/Math/Matrix.hpp"
#include "Nilib/ML/CNodes/CNode.hpp"
#include "Nilib/ML/CNodes/GNode.hpp"
#include "Nilib/ML/Optimizers/Optimizer.hpp"

#include "Nilib/Core/Signal.hpp"
#include "Nilib/Math/Stats.hpp"

namespace Nilib
{
    // A ML supervised dataset. Uses a train/validation/test split of 70%, 20% and 10%.
    template <typename Obs>
    class MLData
    {

    protected:
        struct Range
        {
            using MLdataiterator = std::vector<Obs>::const_iterator;
            MLdataiterator d_begin;
            MLdataiterator d_end;

            Range(MLdataiterator b, MLdataiterator e)
                : d_begin(b), d_end(e) {}

            MLdataiterator begin() const { return d_begin; }
            MLdataiterator end() const { return d_end; }
        };

    public:
        MLData(std::vector<Obs> const &data, size_t const validation_start, size_t const test_start)
            : d_data(data), d_valid_start(validation_start), d_test_start(test_start)
        {
        }

        // Obtain subranges of MLDdata.
        Range train() const { return Range(d_data.begin(), d_data.begin() + d_valid_start); }
        Range validate() const { return Range(d_data.begin() + d_valid_start, d_data.begin() + d_test_start); }
        Range test() const { return Range(d_data.begin() + d_test_start, d_data.end()); }

        // Dimensions of the data.
        inline size_t obs() const { return d_data.size(); };

        inline void shuffle_training() { RNG::shuffle(d_data, 0, d_valid_start); }
        inline void shuffle_validation() { RNG::shuffle(d_data, d_valid_start, d_test_start); }

    private:
        std::vector<Obs> d_data;
        size_t const d_valid_start;
        size_t const d_test_start;
    };

    // Specializations.
    struct XORobs
    {
        static const size_t Xcol = 2;
        static const size_t Ycol = 1;
        Matrixf X, Y;
    };

    MLData<XORobs> XOR();

    struct CIRCLEobs
    {
        static const size_t Xcol = 2;
        static const size_t Ycol = 1;
        Matrixf X, Y;
    };

    MLData<CIRCLEobs> CIRCLE(size_t const ntrain_points = 100);

} // namespace Nilib

#endif