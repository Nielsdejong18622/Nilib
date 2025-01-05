#ifndef _CNODE_PARAM_H
#define _CNODE_PARAM_H

#include "Nilib/ML/CNodes/Input.h"

namespace Nilib
{

    // Input CNode.
    struct Param : public Input
    {
        Param(float value);
    };
}

#endif