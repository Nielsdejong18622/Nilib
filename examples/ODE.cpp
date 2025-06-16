#include "Nilib/Logger/Log.hpp"
#include "Nilib/Logger/CSV.hpp"

#include "Nilib/Math/ODE.hpp"


Nilib::ODE::X myODE(Nilib::ODE::X const &X, float const t)
{
    // y'= y
    return X;
}


int main()
{
    using namespace Nilib;
    using namespace Nilib::ODE;
    X initval = Nilib::Mat<1,1>::ones();
    initval *= 9;

    auto res = Nilib::ODE::EulerMethod(&myODE, 0, 10, initval, 0.01);

    for (size_t idx = 0; idx < res.size(); ++idx)
    {
        auto ODEval = res[idx](0);
        auto realval = initval(0) * exp(idx * 0.01);
        CSV("ODEtest.csv", idx, ODEval, realval);
    }
    
    return 0;
}