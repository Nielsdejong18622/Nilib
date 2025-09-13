#ifndef _GRAPHPOOL_HPP
#define _GRAPHPOOL_HPP

#include "Nilib/ML/CNodes/CNode.hpp"

namespace Nilib
{
    // Flattens n dimensions into F (nxf) -> f (1xf)
    class GraphPoolAverage : public CNode
    {
    public:
        GraphPoolAverage(CNode &X);

        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;

    private:
        CNode &X;
        float d_cached_rows = 0.0f;
    };
} // namespace Nilib
#endif