
#ifndef _CNODE_GRAPHCONV_HPP
#define _CNODE_GRAPHCONV_HPP

#include "Nilib/ML/CNodes/GNode.hpp"

namespace Nilib
{
    // GraphConv.
    class GraphConv : public CNode
    {
    public:
        GraphConv(CNode &A, CNode &X, size_t colX, size_t outdim);

        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;

    private:
        CNode &A;
        CNode &X;
        Weight W;
    };

}

#endif