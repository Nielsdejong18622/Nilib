#ifndef _ML_HPP
#define _ML_HPP

#include "Nilib/ML/CNodes/Weight.h"
#include "Nilib/ML/CNodes/Input.h"

#include "Nilib/ML/CNodes/Concat.h"
#include "Nilib/ML/CNodes/GaussianNoise.h"
#include "Nilib/ML/CNodes/Loss/MSE.h"

#include "Nilib/ML/CNodes/Layer/GraphLayer.h"

#include "Nilib/ML/CNodes/Activation/Activation.h"
#include "Nilib/ML/CNodes/Activation/Sigmoid.h"
#include "Nilib/ML/CNodes/Activation/RBF.h"
#include "Nilib/ML/CNodes/Activation/Relu.h"
#include "Nilib/ML/CNodes/Activation/Selu.h"
#include "Nilib/ML/CNodes/Activation/Tanh.h"

#include "Nilib/ML/Models/Model.h"
#include "Nilib/ML/Models/Perceptron.hpp"
#include "Nilib/ML/Models/MultiLayerPerceptron.hpp"


#include "Nilib/ML/Optimizers/Optimizer.hpp"
#include "Nilib/ML/Optimizers/Adam.hpp"
#include "Nilib/ML/Optimizers/Momentum.hpp"
#include "Nilib/ML/Optimizers/Random.hpp"

#endif