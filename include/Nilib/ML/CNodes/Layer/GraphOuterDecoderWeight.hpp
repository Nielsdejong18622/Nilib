#ifndef _GRAPH_OUTERPRODUCTDECODER_WEIGHT_HPP
#define _GRAPH_OUTERPRODUCTDECODER_WEIGHT_HPP

#include "Nilib/ML/Models/Model.hpp"

namespace Nilib
{
    class OuterProductDecoderWeight : public Module
    {
    public:
        OuterProductDecoderWeight(CNode &X, size_t xcol);

        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;
        void learnables(Nilib::Module::Weights &add) override;

    private:
        CNode &X;
        Weight W;
    };

} // namespace Nilib
#endif