#include "Nilib/ML/MLP.hpp"


int main () {



    MLP<2, 10, 5> NN;

    Nilib::Mat<1, 2> x1(1, 2,{0.0f,0.0f});
    auto pred = NN.forward(&x1);
    LOG_DEBUG("Prediction:", pred);
    pred.print();

    

    return 0;
}