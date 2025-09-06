
#ifndef _CNODE_NOISE_H
#define _CNODE_NOISE_H

#include "Nilib/ML/CNodes/CNode.h"
namespace Nilib
{

    // Noise Gate. Forward pass gets additional Gaussian noise.
    // Do nothing on the backward.
    class GaussianNoise : public CNode
    {
    public:
        GaussianNoise(CNode *input, float const mean = 0.0f, float const sigma = 0.0f);
        void evaluate() override;
        void derive(Matrixf const &seed) override;

    protected:
        CNode *input = nullptr;
        float mean = 0.0f, sigma = 0.0f;
    };
}
#endif