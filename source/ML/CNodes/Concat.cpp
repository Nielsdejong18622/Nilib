#include "Nilib/ML/CNodes/Concat.hpp"

#include "Nilib/Math/LinALg.hpp"

using namespace Nilib;

Nilib::Cbind::Cbind(CNode &lhs, CNode &rhs)
    : lhs(lhs), rhs(rhs)
{
}

void Cbind::evaluate()
{
    lhs.evaluate();
    rhs.evaluate();
    ASSERT(lhs.value.cols() == rhs.value.cols(), lhs.value, rhs.value);
    CORE_ASSERT(lhs.value.rows() > 0);
    CORE_ASSERT(rhs.value.rows() > 0);
    this->value = Nilib::cbind(lhs.value, rhs.value);
}

void Cbind::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.rows() == lhs.value.rows() + rhs.value.rows());
    CORE_ASSERT(seed.cols() == lhs.value.cols());
    CORE_ASSERT(seed.cols() == rhs.value.cols());
    Matrixf seedlhs(lhs.value.rows(), lhs.value.cols());
    Matrixf seedrhs(rhs.value.rows(), rhs.value.cols());

    for (size_t i = 0; i < lhs.value.rows() + rhs.value.rows(); i++)
    {
        for (size_t j = 0; j < seed.cols(); j++)
        {
            if (i < lhs.value.rows())
            {
                seedlhs(i, j) = seed(i, j);
            }
            else
            {
                seedrhs(i - lhs.value.rows(), j) = seed(i, j);
            }
        }
    }

    ASSERT(lhs.value.rows() == seedlhs.rows(), lhs.value, seedlhs);
    ASSERT(lhs.value.cols() == seedlhs.cols(), lhs.value, seedlhs);
    ASSERT(rhs.value.rows() == seedrhs.rows(), rhs.value, seedrhs);
    ASSERT(rhs.value.cols() == seedrhs.cols(), rhs.value, seedrhs);
    lhs.derive(seedlhs);
    rhs.derive(seedrhs);
}

Nilib::Rbind::Rbind(CNode &lhs, CNode &rhs)
    : lhs(lhs), rhs(rhs)
{
}

void Rbind::evaluate()
{
    lhs.evaluate();
    rhs.evaluate();
    CORE_ASSERT(lhs.value.cols() > 0);
    CORE_ASSERT(rhs.value.cols() > 0);
    ASSERT(lhs.value.rows() == rhs.value.rows(), lhs.value, rhs.value);
    this->value = Nilib::rbind(lhs.value, rhs.value);
}

void Rbind::derive(Nilib::Matrixf const &seed)
{
    Matrixf seedlhs(lhs.value.rows(), lhs.value.cols());
    Matrixf seedrhs(rhs.value.rows(), rhs.value.cols());
    for (size_t i = 0; i < lhs.value.rows(); i++)
    {
        for (size_t j = 0; j < lhs.value.cols() + rhs.value.cols(); j++)
        {
            if (j < lhs.value.cols())
            {
                seedlhs(i, j) = seed(i, j);
            }
            else
            {
                seedrhs(i, j - lhs.value.cols()) = seed(i, j);
            }
        }
    }
    ASSERT(lhs.value.rows() == seedlhs.rows(), lhs.value, seedlhs);
    ASSERT(lhs.value.cols() == seedlhs.cols(), lhs.value, seedlhs);
    ASSERT(rhs.value.rows() == seedrhs.rows(), rhs.value, seedrhs);
    ASSERT(rhs.value.cols() == seedrhs.cols(), rhs.value, seedrhs);
    lhs.derive(seedlhs);
    rhs.derive(seedrhs);
}