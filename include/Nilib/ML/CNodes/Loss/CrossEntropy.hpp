#ifndef _CNODE_CROSSENTROPY_HPP
#define _CNODE_CROSSENTROPY_HPP

#include "Nilib/ML/CNodes/CNode.hpp"
#include "Nilib/ML/CNodes/Gates.hpp"
#include "Nilib/ML/CNodes/Activation/Log.hpp"
#include "Nilib/ML/CNodes/Layer/Pool.hpp"

namespace Nilib
{

    class CrossEntropy : public Nilib::CNode
    {
    public:
        CrossEntropy(CNode &prediction, CNode &target);

        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;

    private:
        CNode &prediction;
        CNode &target;

        size_t d_rows, d_cols;
    };
}
#endif