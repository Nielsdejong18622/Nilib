
#ifndef _CNODE_ACTVIVATION_HPP
#define _CNODE_ACTVIVATION_HPP

#include "Nilib/ML/CNodes/CNode.hpp"
#include "Nilib/Math/LinALg.hpp"

namespace Nilib
{
    template <typename ActFun>
    class Activation : public Nilib::CNode
    {
    public:
        Activation(CNode &input)
            : input(input) {}

        void evaluate() override
        {
            input.evaluate();
            this->value = input.value;
            this->value.apply(ActFun::evaluate);
        }

        void derive(Nilib::Matrixf const &seed) override
        {
            CORE_ASSERT(seed.rows() == input.value.rows());
            CORE_ASSERT(seed.cols() == input.value.cols());

            CORE_ASSERT(std::isfinite(seed.sum()));

            input.value.apply(ActFun::derivative);
            input.derive(Nilib::hadamar(seed, input.value));
        }

    private:
        CNode &input;
    };

}
#endif