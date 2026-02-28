
#ifndef _CNODE_GRAPHCONV_HPP
#define _CNODE_GRAPHCONV_HPP

#include "Nilib/ML/Models/Model.hpp"

namespace Nilib
{
// GraphConv.
class GraphConv : public Module
{
  public:
    GraphConv(CNode &A, CNode &X, size_t colX, size_t outdim);

    void evaluate() override;
    void derive(Nilib::Matrixf const &seed) override;

    void learnables(Module::Weights &add) override;

  private:
    CNode &d_A;
    CNode &d_X;
    Weight d_W;
};

} // namespace Nilib

#endif