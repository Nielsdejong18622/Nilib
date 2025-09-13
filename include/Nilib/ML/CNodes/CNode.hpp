#include <iostream>

#include "Nilib/Math/Matrix.hpp"

#ifndef _CNODE_HPP
#define _CNODE_HPP

namespace Nilib
{
    struct CNode
    {
        Nilib::Matrixf value;

        virtual ~CNode() {};

        virtual void evaluate() = 0;
        virtual void derive(Nilib::Matrixf const &seed) = 0;

        friend std::ostream &operator<<(std::ostream &os, CNode const &node);
    };
}

#endif
