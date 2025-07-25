#ifndef _MATRIX_ALG_H
#define _MATRIX_ALG_H

#include <ostream>
#include <sstream>
#include "Nilib/Math/MatrixStorage.hpp"

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Core/Assert.hpp"
#include "Nilib/Math/RNG.hpp"

namespace Nilib
{

    template <typename MatrixData = DynamicMatrixData<float>>
    class Matrix
    {
        MatrixData d_data;

    public:
        auto operator()(size_t const row, size_t const col) const { return d_data(row * d_data.cols() + col); }
        auto &operator()(size_t const row, size_t const col) { return d_data(row * d_data.cols() + col); }
        auto &operator()(size_t const idx) { return d_data(idx); }
        auto operator()(size_t const idx) const { return d_data(idx); }

        size_t rows() const { return d_data.rows(); }
        size_t cols() const { return d_data.cols(); }
        size_t size() const { return d_data.rows() * d_data.cols(); }

        friend std::ostream &operator<<(std::ostream &os, Matrix const &mat)
        {
            if constexpr (std::is_same_v<MatrixData, DynamicMatrixData<float>>)
                return os << "<Matrixf " << mat.rows() << 'x' << mat.cols() << '>';
            else if constexpr (std::is_same_v<MatrixData, DynamicMatrixData<double>>)
                return os << "<Matrixd " << mat.rows() << 'x' << mat.cols() << '>';
            else if constexpr (std::is_same_v<MatrixData, DynamicMatrixData<decltype(mat.sum())>>)
                return os << "<Matrix " << mat.rows() << 'x' << mat.cols() << '>';
            else // Must be static storage type.
                return os << "<Mat " << mat.rows() << 'x' << mat.cols() << '>';
        }

        void print() const
        {
            for (size_t ridx = 0; ridx < d_data.rows(); ++ridx)
            {
                std::stringstream ss;
                ss << "(" << std::fixed; //<< std::setprecision(3);
                for (size_t cidx = 0; cidx < d_data.cols(); ++cidx)
                {

                    if (d_data(ridx, cidx) == std::numeric_limits<float>::max())
                        ss << "INF";
                    else
                        ss << d_data(ridx, cidx);
                    ss << ((cidx < d_data.cols() - 1) ? ',' : ')');
                }
                LOG_DEBUG(ss.str());
            }
        }

        // Returns the sum of all the entries in the Matrix.
        auto sum() const { return d_data.sum(); }
        auto avg() const { return d_data.sum() / size(); }
        Matrix apply(std::function<float(float const)> const &fun) { return d_data.apply(fun); }

        Matrix() = default;
        Matrix(size_t const rows, size_t const cols)
            : d_data(rows, cols) {}
        Matrix(size_t const rows, size_t const cols, MatrixData const &data)
            : d_data(data) {}
        Matrix(MatrixData const &data)
            : d_data(data) {}

        Matrix(std::initializer_list<float> const &data)
            : d_data(data) {}

        // In place operators.
        template <typename data>
        void operator+=(Matrix<data> const &B)
        {
            ASSERT(d_data.rows() == B.d_data.rows(), std::format("{}x{} += {}x{}", d_data.rows(), d_data.cols(), B.rows(), B.cols()));
            ASSERT(d_data.cols() == B.d_data.cols(), std::format("{}x{} += {}x{}", d_data.rows(), d_data.cols(), B.rows(), B.cols()));
            for (size_t idx = 0; idx < d_data.cols() * d_data.rows(); idx++)
                d_data(idx) += B.d_data(idx);
        }

        template <typename data>
        void operator-=(Matrix<data> const &B)
        {
            CORE_ASSERT(d_data.rows() == B.d_data.rows());
            CORE_ASSERT(d_data.cols() == B.d_data.cols());
            for (size_t idx = 0; idx < d_data.cols() * d_data.rows(); idx++)
                d_data(idx) -= B.d_data(idx);
        }

        template <typename scalar>
        void operator*=(scalar const lambda)
            requires(std::is_arithmetic_v<scalar>)
        {
            // d_data *= lambda; // Is defined for valarray.
            for (size_t idx = 0; idx < d_data.cols() * d_data.rows(); idx++)
                d_data(idx) *= lambda;
        }

        template <typename scalar>
        void operator/=(scalar const lambda)
            requires(std::is_arithmetic_v<scalar>)
        {
            for (size_t idx = 0; idx < d_data.cols() * d_data.rows(); idx++)
                d_data(idx) /= lambda;
        }

        void zero()
        {
            fill(0.0);
        }

        void setrandn(float const mean, float const var)
        {
            for (size_t idx = 0; idx < d_data.cols() * d_data.rows(); idx++)
                d_data(idx) = RNG::normal(mean, var);
        }

        void setrand()
        {
            for (size_t idx = 0; idx < d_data.cols() * d_data.rows(); idx++)
                d_data(idx) = RNG::prob();
        }
        // TODO: replace float with storage::type.
        void fill(float const value)
        {
            for (size_t idx = 0; idx < d_data.cols() * d_data.rows(); idx++)
                d_data(idx) = value;
        }

        // static constructors.
        static Matrix all(float const val)
            requires(!std::is_same_v<MatrixData, DynamicMatrixData<float>>)
        {
            Matrix result;
            for (size_t idx = 0; idx < result.cols() * result.rows(); idx++)
                result.d_data(idx) = val;
            return result;
        }

        // static constructors.
        static Matrix all(size_t const n, size_t const m, float const val)
        {
            Matrix result(n, m);
            for (size_t idx = 0; idx < result.cols() * result.rows(); idx++)
                result.d_data(idx) = val;
            return result;
        }
        static Matrix zeros(size_t const n, size_t const m)
        {
            return Matrix::all(n, m, 0.0);
        }

        static Matrix ones(size_t const n, size_t const m)
        {
            return Matrix::all(n, m, 1.0);
        }

        static Matrix zeros()
            requires(!std::is_same_v<MatrixData, DynamicMatrixData<float>>)
        {
            return Matrix::all(0.0);
        }

        static Matrix ones()
            requires(!std::is_same_v<MatrixData, DynamicMatrixData<float>>)
        {
            return Matrix::all(1.0);
        }

        // Creates a random matrix filled with [0-1].
        static Matrix rand(size_t const n, size_t const m)
        {
            Matrix result(n, m);
            for (size_t idx = 0; idx < n * m; ++idx)
                result.d_data(idx) = RNG::prob();
            return result;
        }

        static Matrix rand()
        {
            Matrix result;
            for (size_t idx = 0; idx < result.rows() * result.cols(); ++idx)
                result.d_data(idx) = RNG::prob();
            return result;
        }

        // Creates a random matrix filled with [lower-upper].
        static Matrix randunif(size_t const n, size_t const m, auto const lower, auto const upper)
        {
            Matrix result(n, m);
            for (size_t idx = 0; idx < n * m; ++idx)
                result.d_data(idx) = RNG::uniform(lower, upper);
            return result;
        }

        static Matrix randunif(auto const lower, auto const upper)
        {
            Matrix result;
            for (size_t idx = 0; idx < result.rows() * result.cols(); ++idx)
                result.d_data(idx) = RNG::uniform(lower, upper);
            return result;
        }

        // Creates a random matrix filled with N(mean, sd)
        static Matrix randn(size_t const n, size_t const m, auto const mean, auto const sd)
        {
            Matrix result(n, m);
            for (size_t idx = 0; idx < n * m; ++idx)
                result.d_data(idx) = RNG::normal(mean, sd);
            return result;
        }

        static Matrix randn(auto const mean, auto const sd)
        {
            Matrix result;
            for (size_t idx = 0; idx < result.rows() * result.cols(); ++idx)
                result.d_data(idx) = RNG::normal(mean, sd);
            return result;
        }

        static Matrix diag(size_t const n, size_t const m, float const val)
        {
            Matrix result(n, m);
            for (size_t idx = 0; idx < std::min(n, m); ++idx)
                result(idx, idx) = val;
            return result;
        }

        static Matrix identity(size_t const n)
        {
            return diag(n, n, 1.0);
        }

        // Special access.
        auto x() const { return d_data(0); }
        auto y() const { return d_data(1); }
        auto z() const { return d_data(2); }
        auto a() const { return d_data(3); }
    };

    template <size_t n, size_t m>
    using Mat = Matrix<StaticMatrixData<n, m, float>>;
    using Mat3x3 = Matrix<StaticMatrixData<3, 3, float>>;
    using Matrixf = Matrix<DynamicMatrixData<float>>;
    using Matrixd = Matrix<DynamicMatrixData<double>>;
    using Matrixi = Matrix<DynamicMatrixData<int>>;

    // Special Matrices. (Rotation, Random, Identity, Diagonal, all, etc.)
    using Vec4f = Matrix<StaticMatrixData<4, 1, float>>;
    using Vec3f = Matrix<StaticMatrixData<3, 1, float>>;
    using Vec2f = Matrix<StaticMatrixData<2, 1, float>>;

}

#endif
