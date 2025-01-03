
#ifndef _CNODE_PLUS_H
#define _CNODE_PLUS_H

#include "Nilib/ML/CNodes/CNode.h"
namespace Nilib {

    // Plus gate CNode. 
    struct Plus: public CNode {
        CNode *lhs = nullptr, *rhs = nullptr;
        Plus(CNode *lhs, CNode *rhs): lhs(lhs), rhs(rhs) {}

        void evaluate() {
                lhs->evaluate();
                rhs->evaluate();
                this->value = lhs->value + rhs->value;
        }
        void derive(Matrixf const &seed) {
                lhs->derive(seed);
                rhs->derive(seed);
        }
    };
}
#endif
