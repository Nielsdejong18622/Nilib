#ifndef _MATRIX_ALG_H
#define _MATRIX_ALG_H

#include "Nilib/Math/MatrixStorage.hpp"
#include "Nilib/Math/RNG.hpp"

namespace Nilib
{
    template <typename type = float, typename MatrixData = DynamicMatrixData<type>>
    class Matrix
    {
    public:
        using scalar = type;
        Matrix() = default;
        Matrix(size_t rows, size_t cols) : d_data(rows, cols) {}
        Matrix(size_t rows, size_t cols, type value) : d_data(rows, cols, value) {}
        Matrix(std::initializer_list<type> const &list) : d_data(list) {}
        Matrix(std::initializer_list<std::initializer_list<type>> const &listlist) : d_data(listlist) {}

        static Matrix all(size_t rows, size_t cols, type val) { return Matrix(rows, cols, val); }
        static Matrix zeros(size_t rows, size_t cols) { return all(rows, cols, 0); }
        static Matrix ones(size_t rows, size_t cols) { return all(rows, cols, 1); }
        static Matrix rand(size_t rows, size_t cols) { return randunif(rows, cols, 0.0, 1.0); }
        static Matrix randunif(size_t rows, size_t cols, type min, type max)
        {
            Matrix random(rows, cols);
            random.apply([min, max](type)
                         { return Nilib::RNG::rounded_uniform(min, max); });
            return random;
        }

        // A matrix filled with random normal variables.
        static Matrix randn(size_t rows, size_t cols, type mean, type var)
        {
            Matrix random(rows, cols);
            random.apply([mean, var](type)
                         { return Nilib::RNG::normal<type>(mean, var); });
            return random;
        }

        // A matrix with val on the diagonal, (offdiag) everywhere else.
        static Matrix diag(size_t rows, size_t cols, type val, type offdiag = 0.0)
        {
            Matrix res(rows, cols, offdiag);
            for (size_t idx = 0; idx < std::min(rows, cols); ++idx)
                res(idx, idx) = val;
            return res;
        }
        static Matrix identity(size_t rows, size_t cols) { return diag(rows, cols, 1.0, 0.0); }
        type operator()(size_t row, size_t col) const { return d_data(row * d_data.cols() + col); }
        type &operator()(size_t row, size_t col) { return d_data(row * d_data.cols() + col); }
        type operator()(size_t idx) const { return d_data(idx); }
        type &operator()(size_t idx) { return d_data(idx); }
        type operator[](size_t idx) const { return d_data(idx); }
        type &operator[](size_t idx) { return d_data(idx); }
        inline size_t rows() const { return d_data.rows(); }
        inline size_t cols() const { return d_data.cols(); }
        inline size_t size() const { return d_data.rows() * d_data.cols(); }
        bool operator==(Matrix const &other) { return d_data == other.d_data; }
        friend std::ostream &operator<<(std::ostream &os, Matrix const &mat)
        {
            if constexpr (std::is_same_v<MatrixData, DynamicMatrixData<float>>)
                return os << "<Matrixf " << mat.rows() << 'x' << mat.cols() << '>';
            else if constexpr (std::is_same_v<MatrixData, DynamicMatrixData<double>>)
                return os << "<Matrixd " << mat.rows() << 'x' << mat.cols() << '>';
            else if constexpr (std::is_same_v<MatrixData, DynamicMatrixData<type>>)
                return os << "<Matrix " << mat.rows() << 'x' << mat.cols() << '>';
            else // Must be static storage type.
                return os << "<Mat " << mat.rows() << 'x' << mat.cols() << '>';
        }
        void print() const
        {
            for (size_t ridx = 0; ridx < d_data.rows(); ++ridx)
            {
                std::stringstream ss;
                ss << "(" << std::fixed;
                for (size_t cidx = 0; cidx < d_data.cols(); ++cidx)
                {
                    if (d_data(ridx, cidx) == std::numeric_limits<type>::max())
                        ss << "INF";
                    else
                        ss << d_data(ridx, cidx);
                    ss << ((cidx < d_data.cols() - 1) ? ',' : ')');
                }
                LOG_DEBUG(ss.str());
            }
        }
        // Returns the sum of all the entries in the Matrix.
        type sum() const
        {
            CORE_ASSERT(d_data.size() > 0);
            return d_data.sum();
        }

        inline type avg() const
        {
            type mean = 0.0;
            size_t count = 0;
            for (size_t i = 0; i < d_data.size(); ++i)
            {
                ++count;
                mean += (d_data(i) - mean) / static_cast<type>(count);
            }
            return mean;
        }
        template <typename Callable>
        void apply(Callable &&fun)
        {
            d_data.apply(std::forward<Callable>(fun));
        }

        // In place operators.
        template <typename data>
        void operator+=(Matrix<type, data> const &B)
        {
            ASSERT(d_data.rows() == B.rows(), "A:", d_data.rows(), "x", d_data.cols(), "B:", B.rows(), "x", B.cols());
            ASSERT(d_data.cols() == B.cols(), "A:", d_data.rows(), "x", d_data.cols(), "B:", B.rows(), "x", B.cols());
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) += B.d_data(idx);
        }
        template <typename data>
        void operator-=(Matrix<type, data> const &B)
        {
            CORE_ASSERT(d_data.rows() == B.d_data.rows());
            CORE_ASSERT(d_data.cols() == B.d_data.cols());
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) -= B.d_data(idx);
        }
        template <typename scalar>
        void operator*=(scalar const lambda)
            requires(std::is_arithmetic_v<scalar>)
        {
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) *= lambda;
        }
        template <typename scalar>
        void operator/=(scalar const lambda)
            requires(std::is_arithmetic_v<scalar>)
        {
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) /= lambda;
        }
        void zero() { fill(0.0); }
        void setrandn(type const mean, type const var)
        {
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) = RNG::normal(mean, var);
        }
        void setrand()
        {
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) = RNG::prob();
        }
        void fill(type const value)
        {
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) = value;
        }

        // Generalized x(), y(), z(), and a() methods
        type &x()
            requires(MatrixData::nrow >= 1 && MatrixData::ncol == 1)
        {
            return d_data(0);
        }
        type x() const
            requires(MatrixData::nrow >= 1 && MatrixData::ncol == 1)
        {
            return d_data(0);
        }
        type &y()
            requires(MatrixData::nrow >= 2 && MatrixData::ncol == 1)
        {
            return d_data(1);
        }
        type y() const
            requires(MatrixData::nrow >= 2 && MatrixData::ncol == 1)
        {
            return d_data(1);
        }
        type &z()
            requires(MatrixData::nrow >= 3 && MatrixData::ncol == 1)
        {
            return d_data(2);
        }
        type z() const
            requires(MatrixData::nrow >= 3 && MatrixData::ncol == 1)
        {
            return d_data(2);
        }
        type &a()
            requires(MatrixData::nrow >= 4 && MatrixData::ncol == 1)
        {
            return d_data(3);
        }
        type a() const
            requires(MatrixData::nrow >= 4 && MatrixData::ncol == 1)
        {
            return d_data(3);
        }

    private:
        MatrixData d_data;
    };

    // Special Matrices
    using Mat2x2 = Matrix<float, StaticMatrixData<2, 2, float>>;
    using Mat3x3 = Matrix<float, StaticMatrixData<3, 3, float>>;
    using Mat4x4 = Matrix<float, StaticMatrixData<4, 4, float>>;
    using Vec4f = Matrix<float, StaticMatrixData<4, 1, float>>;
    using Vec3f = Matrix<float, StaticMatrixData<3, 1, float>>;
    using Vec2f = Matrix<float, StaticMatrixData<2, 1, float>>;
    template <size_t n, size_t m>
    using Mat = Matrix<float, StaticMatrixData<n, m, float>>;
    template <typename scalar>
    using Vec2 = Matrix<scalar, StaticMatrixData<2, 1, scalar>>;
    template <typename scalar>
    using Vec3 = Matrix<scalar, StaticMatrixData<3, 1, scalar>>;
    template <typename scalar>
    using Vec4 = Matrix<scalar, StaticMatrixData<4, 1, scalar>>;

    // Short hands.
    using Matrixf = Matrix<float, DynamicMatrixData<float>>;
    using Matrixd = Matrix<double, DynamicMatrixData<double>>;
    using Matrixi = Matrix<int, DynamicMatrixData<int>>;
    template <size_t n>
    using Vecf = Matrix<float, StaticMatrixData<n, 1, float>>;
} // namespace Nilib

#endif
