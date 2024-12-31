#include "Nilib/ML/CNodes/CNode.h"

#include "Nilib/ML/CNodes/Weight.h"
#include "Nilib/ML/CNodes/Input.h"

#include "Nilib/ML/CNodes/Multiply.h"
#include "Nilib/ML/CNodes/Plus.h"

#include "Nilib/ML/CNodes/Regularizer/Regularizer.h"
#include "Nilib/ML/CNodes/Regularizer/L1.h"
#include "Nilib/ML/CNodes/Regularizer/L2.h"

#include "Nilib/ML/CNodes/Loss/Loss.h"
#include "Nilib/ML/CNodes/Loss/MSE.h"

#include "Nilib/ML/CNodes/Layer/DenseLayer.h"

#include "Nilib/ML/CNodes/Activation/Activation.h"
#include "Nilib/ML/CNodes/Activation/Relu.h"
#include "Nilib/ML/CNodes/Activation/Tanh.h"