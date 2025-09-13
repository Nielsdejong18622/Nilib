#ifndef _GATES_HPP
#define _GATES_HPP

#include "Nilib/ML/CNodes/CNode.hpp"

// Some simple gates.

namespace Nilib
{
    struct Plus : public CNode
    {
    public:
        /// @brief X + Y gate.
        Plus(CNode &X, CNode &Y);
        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;

    private:
        CNode &X, &Y;
    };

    struct Minus : public CNode
    {
    public:
        /// @brief X - Y gate.
        Minus(CNode &X, CNode &Y);
        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;

    private:
        CNode &X, &Y;
    };
} // namespace Nilib

#endif