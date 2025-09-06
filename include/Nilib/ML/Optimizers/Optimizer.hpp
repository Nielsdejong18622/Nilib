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
        using Weightptrs = std::vector<Weight *>;

        Optimizer(Weightptrs const &weights);

        virtual void updateGrad(float const multiplier);
        void zeroGrad() const;

        void save(std::string const &filename) const;
        void load(std::string const &filename) const;

    protected:
        bool checkgradients() const;

        Weightptrs weights;
    };

} // namespace Nilib

#endif