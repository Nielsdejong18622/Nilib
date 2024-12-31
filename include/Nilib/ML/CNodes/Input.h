#ifndef _CNODE_INPUT_H
#define _CNODE_INPUT_H

#include "Nilib/ML/CNodes/CNode.h"

namespace Nilib {
    
    // Input CNode. 
    struct Input: public CNode 
    {
        Matrixf partial;
        Input(Matrixf const &value);
        void evaluate() override;
        void derive(Matrixf const &seed) override;
    };
}


#endif