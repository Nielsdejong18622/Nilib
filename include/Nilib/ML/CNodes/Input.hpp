#ifndef _GNODE_INPUT_HPP
#define _GNODE_INPUT_HPP

#include "Nilib/ML/CNodes/GNode.hpp"

namespace Nilib
{

    // CNode that holds a gradient (of size rows * cols), i.e. a Weight node.
    struct Input : public GNode
    {
        Input();
        Input(Matrixf const &value);
        void set(Nilib::Matrixf const &value);
    };

    using Target = Input;
}

#endif