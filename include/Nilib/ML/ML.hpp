#ifndef _ML_HPP
#define _ML_HPP

#include "Nilib/ML/CNodes/CNode.hpp"
#include "Nilib/ML/CNodes/GNode.hpp"
#include "Nilib/ML/CNodes/Input.hpp"

#include "Nilib/ML/CNodes/Concat.hpp"
#include "Nilib/ML/CNodes/GaussianNoise.hpp"
#include "Nilib/ML/CNodes/Loss/MSE.hpp"

#include "Nilib/ML/CNodes/Layer/GraphConv.hpp"
#include "Nilib/ML/CNodes/Layer/GraphOuterDecoder.hpp"
#include "Nilib/ML/CNodes/Layer/GraphPool.hpp"

#include "Nilib/ML/CNodes/Activation/Activation.hpp"
#include "Nilib/ML/CNodes/Activation/Sigmoid.hpp"
#include "Nilib/ML/CNodes/Activation/RBF.hpp"
#include "Nilib/ML/CNodes/Activation/Relu.hpp"
#include "Nilib/ML/CNodes/Activation/Selu.hpp"
#include "Nilib/ML/CNodes/Activation/Tanh.hpp"

#include "Nilib/ML/Models/Model.hpp"
#include "Nilib/ML/Models/Perceptron.hpp"
#include "Nilib/ML/Models/MultiLayerPerceptron.hpp"
#include "Nilib/ML/Models/NeuralNet.hpp"

#include "Nilib/ML/Optimizers/Optimizer.hpp"
#include "Nilib/ML/Optimizers/Adam.hpp"
#include "Nilib/ML/Optimizers/Momentum.hpp"

#include "Nilib/ML/MLData.hpp"
#include "Nilib/ML/MLData.hpp"

#include "Nilib/ML/Supervised.hpp"

#endif