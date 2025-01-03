#include "Nilib/ML/MLP.h"
#include "Nilib/ML/CNodes/Param.h"
#include "Nilib/ML/CNodes/Plus.h"
#include "Nilib/ML/CNodes/Layer/DenseLayer.h"
#include "Nilib/ML/CNodes/Activation/Sigmoid.h"
#include "Nilib/ML/CNodes/Loss/MSE.h"
#include "Nilib/ML/CNodes/Regularizer/L2.h"



namespace Nilib
{

    MLP::MLP(size_t inputdimn, size_t inputdimm, size_t neurons1, size_t neurons2, size_t outputdim)
    : Model({&x}, {&W1, &b1, &W2, &b2}, &out, &y, &TOTLOS),
      labda(0.001f), 
      // X(inputdimn, inputdimm) *
      W1(inputdimm, neurons1),
      b1(inputdimn, neurons1),

      W2(neurons1, neurons2),
      b2(neurons2, outputdim),
      
      H1(&x, &W1, &b1),
      H2(&H1), //0.01f),
      H3(&H2, &W2, &b2),
      out(&H3), // 0.01f),
      MSE(&out, &y),
      REG({&W1, &b1, &W2, &b2}, &labda),
      TOTLOS(&MSE, &REG)
    {
      LOG_INFO("In Ctor!00");

      for (auto &&weight : this->weights)
        LOG_DEBUG(*weight);
    }

  MLP::MLP(MLP const &other)
    : Model({&x}, {&W1, &b1, &W2, &b2}, &out, &y, &TOTLOS),
      labda(0.001f), 
      // X(inputdimn, inputdimm) *
      W1(other.W1),
      b1(other.b1),

      W2(other.W2),
      b2(other.b2),
      
      H1(&x, &W1, &b1),
      H2(&H1),
      H3(&H2, &W2, &b2),
      out(&H3),
      MSE(&out, &y),
      REG({&W1, &b1, &W2, &b2}, &labda),
      TOTLOS(&MSE, &REG)

  {
    LOG_DEBUG("Copy construction!");
  }

}