#ifndef _OPTIMIZER_HPP
#define _OPTIMIZER_HPP

#include "Nilib/Core/Serializer.hpp"
#include "Nilib/Core/Deserializer.hpp"

namespace Nilib
{
    class Weight;

    // In charge of updating the selected weights of a computational graph.
    class Optimizer
    {
    public:
        Optimizer(std::vector<Weight *> weights);

        virtual void updateGrad() const;

        void zeroGrad() const;

        void save(std::string const &filename) const;
        void load(std::string const &filename) const;

    protected:
        bool checkgradients() const;

        std::vector<Weight *> weights;
    };

} // namespace Nilib

#endif