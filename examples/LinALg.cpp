#include "Nilib/Math/LinAlg.hpp"
#include "Nilib/Math/LinAlgFunctions.hpp"
#include "Nilib/Logger/Log.hpp"

#include <assert.h>

using namespace Nilib;

int main()
{
    Matrixf A{{4}};

    Matrixd D({{1,1,1},{-1,2,3}});
    Matrixd D1({{-1,-1,-1},{-1,2,3}});
    Matrixd D2({{-1,-1,-1},{-1,2,3},{0,0,0}});
    Mat3x3 B({1,1,1, 1,1,1, 1.322,1,1});

    B.apply([](float const t) { return std::sin(t + 1);});
    LOG_INFO("Applied sin to B", B);
    B.print();
    LOG_INFO("Matrix A:", A, A.sum());
    A.print();
    LOG_INFO("Multiplying A by 2");
    A *= 2;
    A.print();
    

    LOG_INFO("Dynamic Matrix D:", D, D.sum());
    D.print();
    LOG_INFO("Static  Matrix B:", B, B.sum());
    B.print();
    
    auto C = 1.2 * (D + D1) * 2.0;
    LOG_INFO("Matrix Addition:", C);
    C.print();
    
    C = D - D1;
    LOG_INFO("Matrix Substraction:", C);
    C.print();

    LOG_INFO("D2", D2);
    D2.print();
    auto res = D2 * D2;
    LOG_INFO("Matrix Matrix Multiplication D2xD2", res);
    res.print();

    auto G = Matrixd::diag(4, 6, 4.0);
    auto I = Mat3x3::identity(3);
    LOG_INFO("I", I);
    I.print();
    LOG_INFO("G", G);
    G.print();
    LOG_INFO("transpose(G)", transpose(G));
    transpose(G).print();

    auto R = Matrixd::rand(10,10);
    auto b = Matrixd::rand(10, 1);
    auto c = Matrixd::rand(1, 10);
    auto p = Matrixd::all(10, 10, 10);
    LOG_INFO("P:", p);
    p.print();
    LOG_INFO("Random Matrix R", c * R * b);
    (c * R * b + Matrixd::rand(1,1)).print();

    auto R1 = Mat<10,10>::rand(10,10);
    auto b1 = Mat<10,1>::rand();
    auto c1 = Mat<1,10>::rand();
    LOG_INFO("Random Matrix R1", c1 * R1 * b1);
    auto res1 = (c1 * R1 * b1);
    res1.print();

    // Sizes. 
    LOG_INFO("Size of A", A, sizeof(A), sizeof(size_t), sizeof(std::valarray<float>));
    LOG_INFO("Size of D", D, sizeof(D), sizeof(size_t), sizeof(std::valarray<float>));
    LOG_INFO("Size of c1", c1, sizeof(c1), sizeof(float));
    LOG_INFO("Size of b1", b1, sizeof(b1));
    LOG_INFO("Size of B", B, sizeof(B), sizeof(float), sizeof(std::array<float, 9>));
    LOG_INFO("Size of t(B)", transpose(B), sizeof(transpose(B)));
}