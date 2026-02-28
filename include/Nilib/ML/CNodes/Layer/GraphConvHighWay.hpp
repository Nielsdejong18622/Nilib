
#ifndef _CNODE_GRAPHCONVHIGH_HPP
#define _CNODE_GRAPHCONVHIGH_HPP

#include "Nilib/ML/Models/Model.hpp"

namespace Nilib
{
// GraphConv.
class GraphConvHigh : public Module
{
  public:
    GraphConvHigh(CNode &A, CNode &X, size_t colX, size_t outdim);

    void evaluate() override;
    void derive(Nilib::Matrixf const &seed) override;

    void learnables(Module::Weights &add) override;

  private:
    CNode &d_A;
    CNode &d_X;
    Weight d_W1;
    Weight d_W2;
    float d_alpha = 0.5;
};

} // namespace Nilib

#endif