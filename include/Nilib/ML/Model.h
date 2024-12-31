#ifndef _ML_MODEL_H
#define _ML_MODEL_H

#include "Nilib/ML/CNodes/CNode.h"
#include "Nilib/ML/CNodes/Weight.h"
#include "Nilib/ML/CNodes/Target.h"

class MLData;

namespace Nilib {

    // In charge of updating the weight of a Model.
    class Optimizer
    {
        CNode** weightupdates;
        size_t weights;
    };


    struct Model
    {
        typedef std::vector<Input *> Inputs;
        typedef std::vector<Weight *> Weights;

        // These parameters must be set. 
        Inputs inputs;
        
        // These can be changed to optimize the model.
        Weights weights;

        // This is our approximation and target. 
        CNode *prediction;
        Target *target;

        // How good our model performs. 
        CNode *loss;

    
        Model(Inputs const &inputs, Weights const &weights, CNode *prediction, Target *target, CNode *loss);
        
        // Do a prediction.
        Matrixf &predict(MLData const &data, size_t const idx);
        Matrixf &predict(Matrixf const &x);

        // Zero all gradients.
        void zeroGrad();

        void updateGrad();

        // Fit the model.
        void train(MLData const &data, size_t numepochs);
        //void train(MLEnv);


    };



}

#endif