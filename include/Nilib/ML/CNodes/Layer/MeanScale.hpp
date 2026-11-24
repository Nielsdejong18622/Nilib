#ifndef _MEAN_SCALE_HPP
#define _MEAN_SCALE_HPP

#include "Nilib/ML/CNodes/CNode.hpp"

namespace Nilib
{
  // Scales the matrix X and (approximately) normalizes each entry. 
  // (X - mu) / (var(X) + 0.001)
  class MeanScale : public CNode
  {
  public:
    MeanScale(CNode &X);

    void evaluate() override;
    void derive(Nilib::Matrixf const &seed) override;

  private:
    CNode &d_X;
    Nilib::Matrixf d_mu, d_var;
    size_t d_row, d_col;
  };
} // namespace Nilib
#endif