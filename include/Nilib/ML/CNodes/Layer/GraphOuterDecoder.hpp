#ifndef _GRAPH_OUTERPRODUCTDECODER_HPP
#define _GRAPH_OUTERPRODUCTDECODER_HPP

#include "Nilib/ML/CNodes/CNode.hpp"

namespace Nilib
{
    class OuterProductDecoder : public CNode
    {
    public:
        OuterProductDecoder(CNode &X);

        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;
    private:
        CNode &X;
    };

} // namespace Nilib
#endif