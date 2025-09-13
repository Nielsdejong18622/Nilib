#ifndef _CNODE_PARAM_H
#define _CNODE_PARAM_H

#include "Nilib/ML/CNodes/GNode.hpp"

namespace Nilib
{
    // Single float GNode node.
    struct Param : public GNode
    {
        Param(float const value);
    };
}

#endif