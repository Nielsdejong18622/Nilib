
#ifndef _CNODE_WEIGHT_H
#define _CNODE_WEIGHT_H

#include "Nilib/ML/CNodes/Input.h"

namespace Nilib
{
    // Weight node.
    struct Weight : public Input
    {
        Weight(size_t n, size_t m);
    };

}
#endif
