#include "Nilib/ML/CNodes/Loss/CrossEntropy.hpp"

Nilib::CrossEntropy::CrossEntropy(CNode &prediction, CNode &target)
    : d_prediction(prediction), d_target(target), d_rows(0), d_cols(0)
{
}

void Nilib::CrossEntropy::evaluate()
{
    d_prediction.evaluate();
    d_target.evaluate();
    ASSERT(d_target.value.rows() == d_prediction.value.rows(), d_target.value, "!=", d_prediction.value);
    ASSERT(d_target.value.cols() == d_prediction.value.cols(), d_target.value, "!=", d_prediction.value);
    d_rows = d_target.value.rows();
    d_cols = d_target.value.cols();

    float const epsilon = 1e-12f;
    size_t n = d_prediction.value.size();
    float loss = 0.0;

    for (size_t i = 0; i < n; ++i)
    {
        float x = std::clamp(d_prediction.value(i), epsilon, 1.0f - epsilon);
        float y = d_target.value(i);
        loss += -(y * std::log(x) + (1 - y) * std::log(1 - x));
    }

    this->value = Nilib::Matrixf(1, 1, {loss / static_cast<float>(n)});
}

void Nilib::CrossEntropy::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.rows() == 1);
    CORE_ASSERT(seed.cols() == 1);

    // Undo the meanPool.
    // auto tmp = Nilib::Matrixf::ones(d_rows, d_cols) * seed / static_cast<float>(d_rows * d_cols);

    ASSERT(false, "Not fully implemented yet!");
}
