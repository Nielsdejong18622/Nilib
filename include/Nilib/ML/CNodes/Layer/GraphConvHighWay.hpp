
#ifndef _CNODE_GRAPHCONVHIGH_HPP
#define _CNODE_GRAPHCONVHIGH_HPP

#include "Nilib/ML/Models/Model.hpp"

namespace Nilib
{
    // GraphConv.
    class GraphConvHigh : public Module
    {
    public:
        GraphConvHigh(CNode &A, CNode &X, size_t colX, size_t outdim);

        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;

        void learnables(Module::Weights &add) override;

    private:
        CNode &A;
        CNode &X;
        Weight W1;
        Weight W2;
        float alpha = 0.5;
    };

}

#endif