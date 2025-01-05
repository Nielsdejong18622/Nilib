#ifndef _TARGET_H
#define _TARGET_H

namespace Nilib
{
    // Target CNode.
    struct Target : public Input
    {
        Matrixf partial;
        Target() = default;
        Target(Matrixf const &value);
    };
}

#endif