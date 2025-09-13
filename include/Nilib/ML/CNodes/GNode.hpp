#ifndef _GNODE_HPP
#define _GNODE_HPP

#include "Nilib/ML/CNodes/CNode.hpp"

namespace Nilib
{

    // CNode that holds a gradient (of size rows * cols), i.e. a Weight node.
    struct GNode : public CNode
    {
        Matrixf partial;
        GNode(Matrixf const &value);
        GNode(size_t const rows, size_t const cols);

        void evaluate() override;
        void derive(Matrixf const &seed) override;
    };

    using Weight = GNode;
}

#endif