#include <iostream>

#include "Nilib/Math/LinAlg.hpp"
#include "Nilib/Math/RNG.hpp"
#include "Nilib/Core/Profiler.hpp"

struct CNode {
   Nilib::Matrixf value;
   
   virtual void evaluate() = 0;
   virtual void derive(Nilib::Matrixf const &seed) = 0;

   friend std::ostream &operator<<(std::ostream &os, CNode const &node)
   {    
        os << node.value << '\n';
        node.value.print();
        return os;
   }
};

// Input node. 
struct Input: public CNode {
    Nilib::Matrixf partial;

    Input(Nilib::Matrixf const &value) 
    {
        this->value = value;
        this->partial = Nilib::Matrixf::zeros(value.rows(), value.cols());
    }
    
    void evaluate() 
    {
    }
    
    void derive(Nilib::Matrixf const &seed) 
    {
        partial += seed;
    }
};

// Weight (input) node.
struct Weight : public Input {
    Weight(size_t n, size_t m)
    : Input(Nilib::Matrixf::randn(n, m, 0.0, 1.0))
    {
    }
};

// Plus gate. 
struct Plus: public CNode {
   CNode *lhs, *rhs;
   Plus(CNode *lhs, CNode *rhs): lhs(lhs), rhs(rhs) {}

   void evaluate() {
        lhs->evaluate();
        rhs->evaluate();
        value = lhs->value + rhs->value;
   }
   void derive(Nilib::Matrixf const &seed) {
        lhs->derive(seed);
        rhs->derive(seed);
   }
};

// Multiply Gate. 
struct Multiply: public CNode {
    CNode *lhs, *rhs;
    Multiply(CNode *lhs, CNode *rhs): lhs(lhs), rhs(rhs) {}

    void evaluate() 
    {
        lhs->evaluate();
        rhs->evaluate();
        value = lhs->value * rhs->value;
    }

    void derive(Nilib::Matrixf const &seed) 
    {
        lhs->derive(seed * transpose(rhs->value));
        rhs->derive(transpose(lhs->value) * seed);
    }
};

// Activation gates.
struct Activation : public CNode {
    CNode *input;
    using ActFun = std::function<float(float)>;
    using ActFunDeriv = std::function<float(float)>;
    ActFun actfun;
    ActFunDeriv deriv;

    Activation(CNode *input, ActFun const &fun, ActFunDeriv const &deriv) 
    : input(input), actfun(fun), deriv(deriv) 
    {
    }
    
    void evaluate()
    {
        input->evaluate();
        value = input->value; // Copy the input. 
        value.apply(actfun);
    }

    void derive(Nilib::Matrixf const &seed)
    {
        auto tmp = input->value; // Get a copy of the input. 
        tmp.apply(deriv);
        input->derive(Nilib::hadamar(seed, tmp));
    }
};

struct Tanh: public Activation
{
    Tanh(CNode *input) 
    : Activation(input,
                 std::bind(&Tanh::tanh, this, std::placeholders::_1), 
                 std::bind(&Tanh::tanh_deriv, this, std::placeholders::_1)) {}

    float tanh(float const t) { return std::tanh(t); }
    float tanh_deriv(float const t) { float tmp = std::tanh(t); return 1 - tmp * tmp; }
};

struct Relu: public Activation
{
    float d_leaky;
    Relu(CNode *input, float leaky) 
    : d_leaky(leaky), Activation(input,
                                 std::bind(&Relu::relu, this, std::placeholders::_1), 
                                 std::bind(&Relu::relu_deriv, this, std::placeholders::_1)) {}

    float relu(float const t) {
        return (t > 0) ? t : d_leaky * t;
    }
    float relu_deriv(float const t) {
        return (t > 0) ? 1 : d_leaky;
    }
};

// Composition gates.
struct DenseLayer : public CNode {
    CNode *input;
    CNode *W;
    CNode *b;

    DenseLayer(CNode *input, CNode *W, CNode *b)
    : input(input), W(W), b(b)
    {    
    }

    void evaluate() 
    {
        input->evaluate();
        W->evaluate();
        b->evaluate();
        value = input->value * W->value + b->value;
    }

    void derive(Nilib::Matrixf const &seed)
    {
        input->derive(seed * transpose(W->value));
        W->derive(transpose(input->value) * seed);
        b->derive(seed);
    }

};


// Reshuffle gates.

// Well known loss functions.
struct MSELoss: public CNode 
{
    CNode *prediction;
    CNode *target;

    MSELoss(CNode *prediction, CNode *target)
    : prediction(prediction), target(target) {}

    void evaluate()
    {
        target->evaluate();
        prediction->evaluate();
        auto Error = prediction->value - target->value;
        value = 0.5 * Nilib::hadamar(Error, Error); 
    }

    void derive(Nilib::Matrixf const &seed)
    {
        auto tmp = Nilib::hadamar(prediction->value - target->value, seed);
        prediction->derive(tmp);
        target->derive(-1 * tmp);
    }
};

// Well known loss functions.
struct L1Loss: public CNode 
{
    std::vector<CNode *> weights;
    float labda;

    L1Loss(std::vector<CNode *> const &weights, float const labda)
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
            tmp.apply([](float t) {return std::abs(t);});
            penalty +=  labda * tmp.sum(); // / size;
        }
        value = Nilib::Matrixf(1,1,{penalty}); 
    }

    void derive(Nilib::Matrixf const &seed)
    {
        for (auto &weight : weights)
        {
            auto tmp = weight->value;
            tmp.apply([](float t) { return (t == 0.0)?0.0:((t > 0.0)?1.0:-1.0);});
            weight->derive(labda * tmp);
        }        
    }
};

// Well known loss functions.
struct L2Loss: public CNode 
{
    std::vector<CNode *> weights;
    float labda;

    L2Loss(std::vector<CNode *> const &weights, float const labda)
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
        value = Nilib::Matrixf(1,1,{penalty}); 
    }

    void derive(Nilib::Matrixf const &seed)
    {
        for (auto &weight : weights)
        {
            weight->derive(labda * weight->value);
        }        
    }
};


int main () {
    Nilib::RNG::seed(126);
    
    using namespace Nilib;
    std::vector<Matrixf> features;
    std::vector<Matrixf> output;

    // Create XOR data.
    if (true) {
        features.push_back(Matrixf(1, 2, {0.0, 0.0}));
        features.push_back(Matrixf(1, 2, {1.0, 0.0}));
        features.push_back(Matrixf(1, 2, {0.0, 1.0}));
        features.push_back(Matrixf(1, 2, {1.0, 1.0}));
        
        output.push_back(Matrixf(1, 1, {0.0}));
        output.push_back(Matrixf(1, 1, {1.0}));
        output.push_back(Matrixf(1, 1, {1.0}));
        output.push_back(Matrixf(1, 1, {0.0}));
    } 
    // Sum dataset.
    else if (false)
    {
        for (size_t j = 0; j < 1000; ++j)
        {
            float a, b;
            a = RNG::prob();
            b = RNG::prob();
            features.push_back(Matrixf(1, 2, {a, b}));
            output.push_back(Matrixf(1, 1, {a + b}));
        }
    }
    size_t const epochs = 250'000;

    // Weights and biases to train.
    size_t const inputdim = 2; 
    size_t const neurons1 = 2;
    size_t const neurons2 = 1;
    size_t const outputdim= 1;

    Weight W1(inputdim, neurons1);
    Weight b1(neurons2, neurons1);

    Weight W2(neurons1, neurons2);
    Weight b2(neurons2, outputdim);

    std::vector<CNode*> weights({&W1, &b1, &W2, &b2});

    // For momentum updates.
    float const momentum = 0.2;
    Nilib::Matrixf uW1 = Nilib::Matrixf::zeros(inputdim, neurons1);
    Nilib::Matrixf ub1 = Nilib::Matrixf::zeros(neurons2, neurons1);

    Nilib::Matrixf uW2 = Nilib::Matrixf::zeros(neurons1, neurons2);
    Nilib::Matrixf ub2 = Nilib::Matrixf::zeros(neurons2, outputdim);

    float tolerance = 0.001;
    float epoch_loss = tolerance + 1.0;
    float reg_loss = 0;
    float MSE_loss = 0;

    size_t train_size = 0.8 * features.size();
    size_t i;

    LOG_DEBUG("Network Starting Weights:");
    for (auto &&weight : weights)
        LOG_DEBUG(*weight);

    {
    PROFILE_SCOPE("Network Training");
    
    auto start_time = std::chrono::steady_clock::now();
    for (i = 0; epoch_loss > tolerance && i < epochs; ++i)
    {
        epoch_loss = reg_loss = MSE_loss = 0;

        for (size_t sample = 0; sample < features.size(); ++sample)
        {

            // Zero the gradients.
            W2.partial.zero();
            W1.partial.zero();
            b1.partial.zero();
            b2.partial.zero();

            // Input / Target output.
            Input x(features[sample]);
            Input y(output[sample]);

            // Dense Layer 1. 
            auto H1 = DenseLayer(&x, &W1, &b1);
            //auto H2 = Relu(&H1, 0.01f);
            auto H2 = Tanh(&H1);

            // Dense Layer 2. 
            auto H3 = DenseLayer(&H2, &W2, &b2);
            // auto out = Relu(&H3, 0.01f);
            auto out = Tanh(&H3);
            
            auto MSE = MSELoss(&out, &y);
            auto REG = L1Loss(weights, 0.001);
            auto TOTLOS = Plus(&MSE, &REG);
            // Forward.
            TOTLOS.evaluate();
            
            // Backward.
            TOTLOS.derive(Nilib::Matrixf(1, 1, {1.0f}));
            MSE_loss += MSE.value.sum() / features.size();
            reg_loss += REG.value.sum() / features.size();
            epoch_loss += (reg_loss + MSE_loss) / features.size();

            float lr = 0.05;
            // Clip the gradients so they do not explode. 
            W2.partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
            W1.partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
            b1.partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
            b2.partial.apply([](float t) { return std::clamp(t, -1.0f, 1.0f);});
            
            // (Vanilla) Update rule. 
            /*
            W2.value -= lr * W2.partial;
            W1.value -= lr * W1.partial;
            b1.value -= lr * b1.partial;
            b2.value -= lr * b2.partial;
            */

            // Momentum update rule (defaults to vanilla update when momentum = 0.0).
            uW2 = momentum * uW2 + (lr / features.size()) * W2.partial;
            uW1 = momentum * uW1 + (lr / features.size()) * W1.partial;
            ub2 = momentum * ub2 + (lr / features.size()) * b2.partial;
            ub1 = momentum * ub1 + (lr / features.size()) * b1.partial;
            
            W2.value -= uW2;
            W1.value -= uW1;
            b1.value -= ub1;
            b2.value -= ub2;
        }
        
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
        if (elapsed.count() > 100) {
            LOG_PROGRESS("Epoch", i, "MSE loss:", MSE_loss, "Reg loss:", reg_loss, "total loss:", epoch_loss);
            start_time = current_time;
        }
    }
    } // Profile scope. 
    LOG_DEBUG("Network Final Weights:");
    for (auto &&weight : weights)
        LOG_DEBUG(*weight);
    // After training, do some prediction. 
    for (size_t sample = 0; sample < features.size(); ++sample)
    {
        Input x(features[sample]);
        Input y(output[sample]);

        // Dense Layer 1. 
        auto H1 = DenseLayer(&x, &W1, &b1);
        //auto H2 = Relu(&H1, 0.01f);
        auto H2 = Tanh(&H1);

        // Dense Layer 2. 
        auto H3 = DenseLayer(&H2, &W2, &b2);
        // auto out = Relu(&H3, 0.01f);
        auto out = Tanh(&H3);
        
        // Forward.
        out.evaluate();
        
        LOG_INFO("MLP(", features[sample](0,0), features[sample](0,1), ")=", std::fixed, std::setprecision(4), out.value(0,0), "target", output[sample](0,0));
    }
    return 0;
}