#include <iostream>

#include "Nilib/Math/Matrix.hpp"
#include "Nilib/Math/LinAlgFunctions.hpp"

#ifndef _CNODE_H
#define _CNODE_H

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
