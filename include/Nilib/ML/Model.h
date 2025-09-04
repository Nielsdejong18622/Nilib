#ifndef _ML_MODEL_H
#define _ML_MODEL_H

#include "Nilib/ML/CNodes/CNode.h"
#include "Nilib/ML/CNodes/Weight.h"

#include "Nilib/Core/Serializer.hpp"
#include "Nilib/Core/Deserializer.hpp"

namespace Nilib
{

    // In charge of updating the selected weights of a computational graph.
    class Optimizer
    {
    public:
        Optimizer(std::vector<Weight *> weights)
            : weights(weights) {}

        // Apply the gradients using some rule.
        virtual void step() {};

        void save(std::string const &filename) const
        {
            Serializer writer(filename);
            for (Weight const *w : weights)
                writer.writeMatrix(w->value);
        }
        void load(std::string const &filename) const
        {
            Deserializer loader(filename);
            for (Weight *w : weights)
                loader.readMatrix(w->value);
        }

    protected:
        std::vector<Weight *> weights;
        // Checks if all the weights have gradients initialized!
        bool checkgradients() const
        {
            for (Weight const *w : weights)
                if (w->partial.rows() == 0 || w->partial.cols() == 0)
                    return false;
            return true;
        }
    };

    struct Genetic : public Optimizer
    {

        Genetic(std::vector<Weight *> weights)
            : Optimizer(weights)
        {
        }

        void step() override
        {
            ASSERT(checkgradients(), "One or more gradients are not yet calculated!");
            for (Weight *w : weights)
            {
                w->value += Nilib::Matrixf::randn(w->value.rows(), w->value.cols(), 0.0f, 0.01f);
            }
        }
    };

    class Adam : public Optimizer
    {
        // Make a copy of the weights for the momentum and variance of each parameter.
        std::vector<Weight> momentum;
        std::vector<Weight> variance;

    public:
        Adam(std::vector<Weight *> weights)
            : Optimizer(weights)
        {
        }

        void step() override
        {
        }
    };

    class MLData;
    // A non-owning abstraction over a computational graph.
    class Model
    {
    protected:
        typedef std::vector<Input *> Inputs;
        typedef std::vector<Weight *> Weights;

        // These parameters must be set.
        Inputs inputs;

        // This is our approximation and target.
        CNode *prediction;
        Target *target;

        // How good our model performs.
        CNode *loss;

    public:
        Model(Inputs const &inputs, Weights const &weights, CNode *prediction, Target *target, CNode *loss);
        // These can be changed to optimize the model.
        Weights weights;

        // Do a prediction.
        Matrixf &predict(MLData const &data, size_t const idx);
        Matrixf &predict(Matrixf const &x);

        // Zero all gradients.
        void zeroGrad();

        void updateGrad();

        // Fit the model.
        void train(MLData const &data, size_t numepochs, size_t batch_size);
        // void train(MLEnv);
    };
}

#endif