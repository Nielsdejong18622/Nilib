#include <iostream>

#include "Nilib/Math/LinAlg.hpp"
#include "Nilib/Math/RNG.hpp"
#include "Nilib/Core/Profiler.hpp"

#ifndef _CNODE_H
#define _CNODE_H

struct CNodeBase
{

};

template<typename Matrix = Nilib::Matrix<Nilib::DynamicMatrixData<float>>>
struct CNode : public CNodeBase{
    Matrix value;

    virtual void evaluate() = 0;
    virtual void derive(Matrix const &seed) = 0;

    friend std::ostream &operator<<(std::ostream &os, CNode const &node) 
    {    
        os << node.value << '\n';
        node.value.print();
        return os;
    }
};

#endif

#ifndef _CNODE_INPUT_H
#define _CNODE_INPUT_H

template<typename Matrix = Nilib::Matrix<>>
struct Input: public CNode<Matrix> {
    Matrix partial;

    Input(Matrix const &value) 
    {
        this->value = value;
        this->partial = Matrix::zeros(value.rows(), value.cols());
        // this->partial.zero();
    }
    
    void evaluate() 
    {
    }
    
    void derive(Matrix const &seed) 
    {
        partial += seed;
    }
};

#endif

#ifndef _CNODE_WEIGHT_H
#define _CNODE_WEIGHT_H

// Weight (input) node.
template<typename Matrix = Nilib::Matrix<>>
struct Weight : public Input<Matrix> {
    Weight(size_t n, size_t m)
    : Input<Matrix>(Matrix::randn(n, m, 0.0, 1.0))
    {
    }
};

#endif

#ifndef _CNODE_PLUS_H
#define _CNODE_PLUS_H
// Plus gate CNode. 
template<typename Matrix = Nilib::Matrix<>>
struct Plus: public CNode<Matrix> {
   CNode<Matrix> *lhs, *rhs;
   Plus(CNode<Matrix> *lhs, CNode<Matrix> *rhs): lhs(lhs), rhs(rhs) {}

   void evaluate() {
        lhs->evaluate();
        rhs->evaluate();
        this->value = lhs->value + rhs->value;
   }
   void derive(Matrix const &seed) {
        lhs->derive(seed);
        rhs->derive(seed);
   }
};
#endif

#ifndef _CNODE_MULTIPLY_H
#define _CNODE_MULTIPLY_H
// Multiply Gate. 
template<typename Matrix = Nilib::Matrix<>>
struct Multiply: public CNode<Matrix> {
    CNode<Matrix> *lhs, *rhs;
    Multiply(CNode<Matrix> *lhs, CNode<Matrix> *rhs): lhs(lhs), rhs(rhs) {}

    void evaluate() 
    {
        lhs->evaluate();
        rhs->evaluate();
        this->value = lhs->value * rhs->value;
    }

    void derive(Matrix const &seed) 
    {
        lhs->derive(seed * transpose(rhs->value));
        rhs->derive(transpose(lhs->value) * seed);
    }
};
#endif

#ifndef _CNODE_ACTVIVATION_H
#define _CNODE_ACTVIVATION_H
// Activation gates.
template<typename Matrix = Nilib::Matrix<>>
struct Activation : public CNode<Matrix> {
    CNode<Matrix> *input;
    using ActFun = std::function<float(float)>;
    using ActFunDeriv = std::function<float(float)>;
    ActFun actfun;
    ActFunDeriv deriv;

    Activation(CNode<Matrix> *input, ActFun const &fun, ActFunDeriv const &deriv) 
    : input(input), actfun(fun), deriv(deriv) 
    {
    }
    
    void evaluate()
    {
        input->evaluate();
        this->value = input->value; // Copy the input. 
        this->value.apply(actfun);
    }

    void derive(Matrix const &seed)
    {
        auto tmp = input->value; // Get a copy of the input. 
        tmp.apply(deriv);
        input->derive(Nilib::hadamar(seed, tmp));
    }
};
#endif

#ifndef _CNODE_TANH_H
#define _CNODE_TANH_H
template<typename Matrix = Nilib::Matrix<>>
struct Tanh: public Activation<Matrix>
{
    Tanh(CNode<Matrix> *input) 
    : Activation<Matrix>(input,
                 std::bind(&Tanh::tanh, this, std::placeholders::_1), 
                 std::bind(&Tanh::tanh_deriv, this, std::placeholders::_1)) {}

    float tanh(float const t) { return std::tanh(t); }
    float tanh_deriv(float const t) { float tmp = std::tanh(t); return 1 - tmp * tmp; }
};

#endif

#ifndef _CNODE_RELU_H
#define _CNODE_RELU_H
template<typename Matrix = Nilib::Matrix<>>
struct Relu: public Activation<Matrix>
{
    float d_leaky;
    Relu(CNode<Matrix> *input, float leaky) 
    : d_leaky(leaky), Activation<Matrix>(input,
                                 std::bind(&Relu::relu, this, std::placeholders::_1), 
                                 std::bind(&Relu::relu_deriv, this, std::placeholders::_1)) {}

    float relu(float const t) {
        return (t > 0) ? t : d_leaky * t;
    }
    float relu_deriv(float const t) {
        return (t > 0) ? 1 : d_leaky;
    }
};
#endif

#ifndef _CNODE_DENSELAYER_H
#define _CNODE_DENSELAYER_H
// Composition gates.
struct DenseLayer : public CNode<Mat<> {

    DenseLayer(CNodeBase *input, CNodeBase *W, CNodeBase *b)
    : input(input), W(W), b(b)
    {    
    }

    void evaluate() 
    {
        static_cast<CNode<Matrixinput->evaluate();
        W->evaluate();
        b->evaluate();
        this->value = input->value * W->value + b->value;
    }

    void derive(Matrix const &seed)
    {
        input->derive(seed * transpose(W->value));
        W->derive(transpose(input->value) * seed);
        b->derive(seed);
    }

};
#endif

#ifndef _CNODE_MSELOSS_H
#define _CNODE_MSELOSS_H
// Well known loss functions.
template<typename Matrix = Nilib::Matrix<>>
struct MSELoss: public CNode<Matrix> 
{
    CNode<Matrix> *prediction;
    CNode<Matrix> *target;

    MSELoss(CNode<Matrix> *prediction, CNode<Matrix> *target)
    : prediction(prediction), target(target) {}

    void evaluate()
    {
        target->evaluate();
        prediction->evaluate();
        auto Error = prediction->value - target->value;
        this->value = 0.5 * Nilib::hadamar(Error, Error); 
    }

    void derive(Matrix const &seed)
    {
        auto tmp = Nilib::hadamar(prediction->value - target->value, seed);
        prediction->derive(tmp);
        target->derive(-1 * tmp);
    }
};
#endif


#ifndef _CNODE_L1LOSS_H
#define _CNODE_L1LOSS_H
// Well known loss functions.
template<typename Matrix = Nilib::Matrix<>>
struct L1Loss: public CNode<Matrix> 
{
    std::vector<CNodeBase*> weights;
    float labda;

    L1Loss(std::initializer_list<CNodeBase *> const &weights, float const labda)
    : weights(weights), labda(labda) {}

    void evaluate()
    {
        // Loop over all the weights and penalize their size.  
        float penalty = 0;
        for (size_t wi = 0; wi < weights.size(); ++wi)
        {
            CNode<Matrix>* weight = static_cast<CNode<Matrix>*>(weights[wi]);
            weight->evaluate();
            //size_t size = weight->value.rows() * weight->value.cols();
            auto tmp = weight->value; // Copy!
            tmp.apply([](float t) {return std::abs(t);});
            penalty +=  labda * tmp.sum(); // / size;
        }
        this->value = Matrix(1,1,{penalty}); 
    }

    void derive(Matrix const &seed)
    {
        for (size_t wi = 0; wi < weights.size(); ++wi)
        {
            CNode<Matrix>* weight = static_cast<CNode<Matrix>*>(weights[wi]);
            Matrix tmp = weight->value;
            tmp.apply([](float t) { return (t == 0.0)?0.0:((t > 0.0)?1.0:-1.0);});
            weight->derive(labda * tmp);
        }        
    }
};
#endif

#ifndef _CNODE_L2LOSS_H
#define _CNODE_L2LOSS_H
// Well known loss functions.

template<typename Matrix = Nilib::Matrix<>>
struct L2Loss: public CNode<Matrix> 
{
    std::vector<CNode<Matrix> *> weights;
    float labda;

    L2Loss(std::vector<CNode<Matrix> *> const &weights, float const labda)
    : weights(weights), labda(labda) {}

    void evaluate()
    {
        // Loop over all the weights and penalize their size.  
        float penalty = 0;
        for (size_t wi = 0; wi < weights.size(); ++wi)
        {
            weights[wi]->evaluate();
            //size_t size = weight->value.rows() * weight->value.cols();
            auto tmp = weights[wi]->value; // Copy!
            tmp.apply([](float t) {return 0.5 * t * t;});
            penalty +=  labda * tmp.sum(); // / size;
        }
        this->value = Matrix(1,1,{penalty}); 
    }

    void derive(Matrix const &seed)
    {
        for (auto &weight : weights)
        {
            weight->derive(labda * weight->value);
        }        
    }
};
#endif
