#ifndef _ML_MODEL_H
#define _ML_MODEL_H

#include "Nilib/ML/CNodes/CNode.h"
#include "Nilib/ML/CNodes/Weight.h"
#include "Nilib/ML/CNodes/Target.h"

namespace Nilib
{

    class MLData;

    // In charge of updating the weight of a Model.
    class Optimizer
    {
        CNode **weightupdates;
        size_t weights;
    };

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