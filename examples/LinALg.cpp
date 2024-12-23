#include "Nilib/Math/LinAlg.hpp"
#include "Nilib/Logger/Log.hpp"

using namespace Nilib;

int main()
{
    
    Matrixf A{{4}};
    Matrixd D({{1,1,1},{-1,2,3}});
    Matrixd D1({{-1,-1,-1},{-1,2,3}});
    Matrixd D2({{-1,-1,-1},{-1,2,3},{0,0,0}});
    Mat3x3 B({1,1,1, 1,1,1, 1.322,1,1});

    B.d_data.apply([](float const t) { return std::sin(t + 1);});
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
    auto res = D2*D2 *B;
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
}