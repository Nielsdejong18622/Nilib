
#ifndef _CNODE_NOISE_H
#define _CNODE_NOISE_H

#include "Nilib/ML/CNodes/CNode.h"
namespace Nilib
{

    // Noise Gate. Forward pass gets additional Gaussian noise.
    // Do nothing on the backward.
    struct GaussianNoise : public CNode
    {
        CNode *input = nullptr;
        float mean = 0.0f, sigma = 0.0f;
        GaussianNoise(CNode *input, float const mean = 0.0f, float const sigma = 0.0f) : input(input), mean(mean), sigma(sigma) {}

        void evaluate()
        {
            input->evaluate();
            this->value = input->value + Nilib::Matrixf::randn(input->value.rows(), input->value.cols(), mean, sigma);
        }

        void derive(Matrixf const &seed)
        {
            input->derive(seed);
        }
    };
}
#endif