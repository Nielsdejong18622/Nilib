#include "Nilib/ML/CNodes/CNode.h"

namespace Nilib {

    std::ostream &operator<<(std::ostream &os, CNode const &node)
    {    
        os << node.value << '\n';
        node.value.print();
        return os;
    }
}
