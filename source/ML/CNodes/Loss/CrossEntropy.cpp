#include "Nilib/ML/CNodes/Loss/CrossEntropy.hpp"

Nilib::CrossEntropy::CrossEntropy(CNode &prediction, CNode &target)
    : prediction(prediction), target(target), d_rows(0), d_cols(0)
{
}

void Nilib::CrossEntropy::evaluate()
{
    prediction.evaluate();
    target.evaluate();
    ASSERT(target.value.rows() == prediction.value.rows(), target.value, "!=", prediction.value);
    ASSERT(target.value.cols() == prediction.value.cols(), target.value, "!=", prediction.value);
    d_rows = target.value.rows();
    d_cols = target.value.cols();

    float const epsilon = 1e-12;
    size_t n = prediction.value.size();
    float loss = 0.0;

    for (size_t i = 0; i < n; ++i)
    {
        float x = std::clamp(prediction.value(i), epsilon, 1.0f - epsilon);
        float y = target.value(i);
        loss += -(y * std::log(x) + (1 - y) * std::log(1 - x));
    }

    this->value = Nilib::Matrixf(1, 1, {loss / static_cast<float>(n)});
}

void Nilib::CrossEntropy::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.rows() == 1);
    CORE_ASSERT(seed.cols() == 1);

    // Undo the meanPool.
    auto tmp = Nilib::Matrixf::ones(d_rows, d_cols) * seed / static_cast<float>(d_rows * d_cols);

    ASSERT(false, "Not fully implemented yet!");
}
