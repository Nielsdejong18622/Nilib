#ifndef _MEAN_POOL_HPP
#define _MEAN_POOL_HPP

#include "Nilib/ML/CNodes/CNode.hpp"

namespace Nilib
{
    // Flattens nxn dimensions into 1x1
    class MeanPool : public CNode
    {
    public:
        MeanPool(CNode &X);

        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;

    private:
        CNode &X;
        size_t d_row, d_col;
    };
} // namespace Nilib
#endif