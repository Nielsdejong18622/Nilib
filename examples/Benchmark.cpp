#include "Nilib/Core/Benchmark.hpp"
#include "Nilib/Math/LinAlg.hpp"
#include "Nilib/Math/LinALg.hpp"

double square(double x)
{
    return x * x;
}

template <typename scalar>
scalar cube(scalar const &x)
{
    return x * x * x;
}

int main()
{
    using namespace Nilib;

    double input = 123.456;
    Mat2x2 mat = Mat2x2::rand();
    Matrixf mat_dyn = Matrixf::rand(2, 2);
    size_t iters = 1000000;

    LOG_DEBUG(mat_dyn);
    mat_dyn.print();
    auto A = mat_dyn * mat_dyn * mat_dyn;
    A.print();  
    BenchMarkEnv benchmark(1000000, 10, "Multiplication_bench.csv");

    benchmark.function("Cube_double", cube<double>, input);
    benchmark.function("Cube_int", cube<int>, 123);
    benchmark.function("Cube_max", cube<Mat2x2>, mat);
    benchmark.function("Cube_max_dyn", cube<Matrixf>, mat_dyn);

    return 0;
}