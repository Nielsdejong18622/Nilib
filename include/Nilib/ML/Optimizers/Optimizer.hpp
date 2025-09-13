#ifndef _OPTIMIZER_HPP
#define _OPTIMIZER_HPP

#include "Nilib/Core/Serializer.hpp"
#include "Nilib/Core/Deserializer.hpp"

#include "Nilib/ML/Models/Model.hpp"

namespace Nilib
{

    // In charge of updating the selected weights of a computational graph.
    class Optimizer
    {
    public:
        using Weightptrs = Module::Weights;

        Optimizer(Module *model);
        Optimizer(Weightptrs const &weights);

        static Optimizer fromDisk(std::string const &weight_file);

        virtual void updateGrad(float const multiplier = 1.0f);
        void zeroGrad() const;

        void initWeights() const;
        void printWeights() const;

        void save(std::string const &filename) const;
        void load(std::string const &filename) const;

    protected:
        bool checkgradients() const;

        Weightptrs weights;
    };

} // namespace Nilib

#endif