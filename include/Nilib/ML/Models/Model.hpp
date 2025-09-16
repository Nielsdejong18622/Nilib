#ifndef _ML_MODEL_HPP
#define _ML_MODEL_HPP

#include "Nilib/ML/CNodes/GNode.hpp"

namespace Nilib
{

    struct Module : public CNode
    {
        // typedef std::vector<GNode *> Weights;

        class Weights
        {
        public:
            using iterator = std::vector<GNode *>::iterator;
            using const_iterator = std::vector<GNode *>::const_iterator;

            // Range-based for loop support
            iterator begin() { return weights.begin(); }
            iterator end() { return weights.end(); }
            const_iterator begin() const { return weights.begin(); }
            const_iterator end() const { return weights.end(); }
            const_iterator cbegin() const { return weights.cbegin(); }
            const_iterator cend() const { return weights.cend(); }

            size_t size() const { return weights.size(); }

            void push_back(GNode *add) { weights.push_back(add); }
            GNode *operator[](size_t idx) { return weights[idx]; }
            GNode *const operator[](size_t idx) const { return weights[idx]; }

            Weights(Module &module) { module.learnables(*this); }
            Weights() = default;

        private:
            std::vector<GNode *> weights;
        };

        virtual void learnables(Weights &add) = 0;
    };

    // // An owning abstraction over a computational graph.
    // class Module
    // {
    // protected:
    //     typedef std::vector<GNode *> Inputs;

    //     // These parameters must be set.
    //     Inputs inputs;

    //     // This is our approximation and target.
    //     CNode *prediction;
    //     Target *target;

    //     // How good our model performs.
    //     CNode *loss;

    // public:
    //     Module(Inputs const &inputs, Weights const &weights, CNode *prediction, Target *target, CNode *loss);
    //     // These can be changed to optimize the model.
    //     Weights weights;

    //     // Do a prediction.
    //     Matrixf &predict(MLData const &data, size_t const idx);
    //     Matrixf &predict(Matrixf const &x);

    //     // Zero all gradients.
    //     void zeroGrad();

    //     void updateGrad();

    //     // Fit the model.
    //     // void train(MLData const &data, size_t numepochs, size_t batch_size);
    //     // void train(MLEnv);
    // };
}

#endif