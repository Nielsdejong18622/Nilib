#include "Nilib/ML/CNodes/Concat.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

Nilib::Cbind::Cbind(CNode &lhs, CNode &rhs) : d_lhs(lhs), d_rhs(rhs)
{
}

void Cbind::evaluate()
{
    d_lhs.evaluate();
    d_rhs.evaluate();
    ASSERT(d_lhs.value.cols() == d_rhs.value.cols(), d_lhs.value, d_rhs.value);
    CORE_ASSERT(d_lhs.value.rows() > 0);
    CORE_ASSERT(d_rhs.value.rows() > 0);
    this->value = Nilib::cbind(d_lhs.value, d_rhs.value);
}

void Cbind::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.rows() == d_lhs.value.rows() + d_rhs.value.rows());
    CORE_ASSERT(seed.cols() == d_lhs.value.cols());
    CORE_ASSERT(seed.cols() == d_rhs.value.cols());
    Matrixf seedlhs(d_lhs.value.rows(), d_lhs.value.cols());
    Matrixf seedrhs(d_rhs.value.rows(), d_rhs.value.cols());

    for (size_t i = 0; i < d_lhs.value.rows() + d_rhs.value.rows(); i++)
    {
        for (size_t j = 0; j < seed.cols(); j++)
        {
            if (i < d_lhs.value.rows())
            {
                seedlhs(i, j) = seed(i, j);
            }
            else
            {
                seedrhs(i - d_lhs.value.rows(), j) = seed(i, j);
            }
        }
    }

    ASSERT(d_lhs.value.rows() == seedlhs.rows(), d_lhs.value, seedlhs);
    ASSERT(d_lhs.value.cols() == seedlhs.cols(), d_lhs.value, seedlhs);
    ASSERT(d_rhs.value.rows() == seedrhs.rows(), d_rhs.value, seedrhs);
    ASSERT(d_rhs.value.cols() == seedrhs.cols(), d_rhs.value, seedrhs);
    d_lhs.derive(seedlhs);
    d_rhs.derive(seedrhs);
}

Nilib::Rbind::Rbind(CNode &lhs, CNode &rhs) : d_lhs(lhs), d_rhs(rhs)
{
}

void Rbind::evaluate()
{
    d_lhs.evaluate();
    d_rhs.evaluate();
    CORE_ASSERT(d_lhs.value.cols() > 0);
    CORE_ASSERT(d_rhs.value.cols() > 0);
    ASSERT(d_lhs.value.rows() == d_rhs.value.rows(), d_lhs.value, d_rhs.value);
    this->value = Nilib::rbind(d_lhs.value, d_rhs.value);
}

void Rbind::derive(Nilib::Matrixf const &seed)
{
    Matrixf seedlhs(d_lhs.value.rows(), d_lhs.value.cols());
    Matrixf seedrhs(d_rhs.value.rows(), d_rhs.value.cols());
    for (size_t i = 0; i < d_lhs.value.rows(); i++)
    {
        for (size_t j = 0; j < d_lhs.value.cols() + d_rhs.value.cols(); j++)
        {
            if (j < d_lhs.value.cols())
            {
                seedlhs(i, j) = seed(i, j);
            }
            else
            {
                seedrhs(i, j - d_lhs.value.cols()) = seed(i, j);
            }
        }
    }
    ASSERT(d_lhs.value.rows() == seedlhs.rows(), d_lhs.value, seedlhs);
    ASSERT(d_lhs.value.cols() == seedlhs.cols(), d_lhs.value, seedlhs);
    ASSERT(d_rhs.value.rows() == seedrhs.rows(), d_rhs.value, seedrhs);
    ASSERT(d_rhs.value.cols() == seedrhs.cols(), d_rhs.value, seedrhs);
    d_lhs.derive(seedlhs);
    d_rhs.derive(seedrhs);
}