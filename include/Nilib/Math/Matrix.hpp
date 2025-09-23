#ifndef _MATRIX_ALG_H
#define _MATRIX_ALG_H

#include "Nilib/Math/MatrixStorage.hpp"

#include "Nilib/Math/RNG.hpp"

namespace Nilib
{

    template <typename MatrixData, typename ValueType>
    concept IsDynamicMatrixData = std::is_same_v<MatrixData, DynamicMatrixData<ValueType>> || std::is_same_v<MatrixData, SparseMatrixData<ValueType>>;

    template <typename value_type = float, typename MatrixData = DynamicMatrixData<value_type>>
    class Matrix
    {
        MatrixData d_data;

    public:
        using scalar = value_type;
        template <typename OtherScalar, typename OtherMatrixData>
        Matrix(Matrix<OtherScalar, OtherMatrixData> const &other)
        {
            size_t rows = other.rows();
            size_t cols = other.cols();
            for (size_t i = 0; i < rows; ++i)
            {
                for (size_t j = 0; j < cols; ++j)
                {
                    d_data(i, j) = static_cast<value_type>(other(i, j));
                }
            }
        }

        value_type operator()(size_t const row, size_t const col) const { return d_data(row * d_data.cols() + col); }
        value_type &operator()(size_t const row, size_t const col) { return d_data(row * d_data.cols() + col); }
        value_type &operator()(size_t const idx) { return d_data(idx); }
        value_type operator()(size_t const idx) const { return d_data(idx); }

        inline size_t rows() const { return d_data.rows(); }
        inline size_t cols() const { return d_data.cols(); }
        inline size_t size() const { return d_data.rows() * d_data.cols(); }

        friend std::ostream &operator<<(std::ostream &os, Matrix const &mat)
        {
            if constexpr (std::is_same_v<MatrixData, DynamicMatrixData<float>>)
                return os << "<Matrixf " << mat.rows() << 'x' << mat.cols() << '>';
            else if constexpr (std::is_same_v<MatrixData, DynamicMatrixData<double>>)
                return os << "<Matrixd " << mat.rows() << 'x' << mat.cols() << '>';
            else if constexpr (std::is_same_v<MatrixData, DynamicMatrixData<value_type>>)
                return os << "<Matrix " << mat.rows() << 'x' << mat.cols() << '>';
            else if constexpr (std::is_same_v<MatrixData, SparseMatrixData<value_type>>)
                return os << "<SparseMatrix " << mat.rows() << 'x' << mat.cols() << '>';
            else // Must be static storage type.
                return os << "<Mat " << mat.rows() << 'x' << mat.cols() << '>';
        }

        void print() const
        {
            for (size_t ridx = 0; ridx < d_data.rows(); ++ridx)
            {
                std::stringstream ss;
                ss << "(" << std::fixed << std::setprecision(3);
                for (size_t cidx = 0; cidx < d_data.cols(); ++cidx)
                {
                    if (d_data(ridx, cidx) == std::numeric_limits<value_type>::max())
                        ss << "INF";
                    else
                        ss << d_data(ridx, cidx);
                    ss << ((cidx < d_data.cols() - 1) ? ',' : ')');
                }
                LOG_DEBUG(ss.str());
            }
        }

        // Returns the sum of all the entries in the Matrix.
        value_type sum() const
        {
            CORE_ASSERT(d_data.size() > 0);
            return d_data.sum();
        }

        inline value_type avg() const
        {
            value_type mean = 0.0;
            size_t count = 0;
            for (size_t i = 0; i < d_data.size(); ++i)
            {
                ++count;
                mean += (d_data(i) - mean) / count;
            }
            return mean;
        }

        inline value_type stddev() const
        {
            if (d_data.size() < 2)
                return 0.0; // Standard deviation is undefined for size < 2

            value_type mean = 0.0;
            value_type M2 = 0.0;
            size_t count = 0;

            for (size_t i = 0; i < d_data.size(); ++i)
            {
                ++count;
                value_type x = d_data(i);
                value_type delta = x - mean;
                mean += delta / count;
                value_type delta2 = x - mean;
                M2 += delta * delta2;
            }

            return std::sqrt(M2 / (count - 1)); // Sample standard deviation
        }
        template <typename Callable>
        void apply(Callable &&fun) { d_data.apply(std::forward<Callable>(fun)); }

        Matrix() = default;
        Matrix(size_t const rows, size_t const cols)
            // requires(std::is_same<MatrixData, DynamicMatrixData<value_type>>::value)
            : d_data(rows, cols)
        {
        }
        Matrix(size_t const rows, size_t const cols, MatrixData const &data)
            // requires(std::is_same<MatrixData, DynamicMatrixData<value_type>>::value)
            : d_data(rows, cols, data.d_data)
        {
        }

        Matrix(std::initializer_list<value_type> const &data)
            requires(std::is_arithmetic<value_type>::value)
            : d_data(data)
        {
        }
        Matrix(std::initializer_list<std::initializer_list<value_type>> const &data)
            requires(std::is_arithmetic<value_type>::value)
            : d_data(data)
        {
        }

        // In place operators.
        template <typename data>
        void operator+=(Matrix<value_type, data> const &B)
        {
            ASSERT(d_data.rows() == B.rows(), "A:", d_data.rows(), "x", d_data.cols(), "B:", B.rows(), "x", B.cols());
            ASSERT(d_data.cols() == B.cols(), "A:", d_data.rows(), "x", d_data.cols(), "B:", B.rows(), "x", B.cols());

            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) += B.d_data(idx);
        }

        template <typename data>
        void operator-=(Matrix<value_type, data> const &B)
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

        void zero()
        {
            fill(0.0);
        }

        void setrandn(value_type const mean, value_type const var)
        {
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) = RNG::normal(mean, var);
        }

        void setrand()
        {
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) = RNG::prob();
        }

        void fill(value_type const value)
        {
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) = value;
        }

        // static constructors.
        [[nodiscard]] static Matrix all(size_t const n, size_t const m, value_type const val)
            requires(IsDynamicMatrixData<MatrixData, value_type>)
        {
            Matrix result(n, m);
            result.fill(val);
            return result;
        }
        [[nodiscard]] static Matrix all(value_type const val)
            requires(!IsDynamicMatrixData<MatrixData, value_type>)
        {
            Matrix result;
            result.fill(val);
            return result;
        }

        [[nodiscard]] static Matrix zeros(size_t const n, size_t const m)
            requires(IsDynamicMatrixData<MatrixData, value_type>)
        {
            return Matrix::all(n, m, 0.0);
        }
        [[nodiscard]] static Matrix zeros()
            requires(!IsDynamicMatrixData<MatrixData, value_type>)
        {
            return Matrix::all(0.0);
        }

        [[nodiscard]] static Matrix ones(size_t const n, size_t const m)
            requires(IsDynamicMatrixData<MatrixData, value_type>)
        {
            return Matrix::all(n, m, 1.0);
        }
        [[nodiscard]] static Matrix ones()
            requires(!IsDynamicMatrixData<MatrixData, value_type>)
        {
            return Matrix::all(1.0);
        }

        // Creates a random matrix filled with [0-1].
        [[nodiscard]] static Matrix rand(size_t const n, size_t const m)
            requires(IsDynamicMatrixData<MatrixData, value_type>)
        {
            Matrix result(n, m);
            for (size_t idx = 0; idx < n * m; ++idx)
                result.d_data(idx) = RNG::prob();
            return result;
        }
        [[nodiscard]] static Matrix rand()
            requires(!IsDynamicMatrixData<MatrixData, value_type>)
        {
            Matrix result;
            for (size_t idx = 0; idx < result.size(); ++idx)
                result.d_data(idx) = RNG::prob();
            return result;
        }

        // Creates a random matrix filled with [lower-upper].
        [[nodiscard]] static Matrix randunif(size_t const n, size_t const m, value_type const lower, value_type const upper)
            requires(IsDynamicMatrixData<MatrixData, value_type>)
        {
            Matrix result(n, m);
            for (size_t idx = 0; idx < n * m; ++idx)
                result.d_data(idx) = RNG::uniform(lower, upper);
            return result;
        }
        [[nodiscard]] static Matrix randunif(value_type const lower, value_type const upper)
            requires(!IsDynamicMatrixData<MatrixData, value_type>)
        {
            Matrix result;
            for (size_t idx = 0; idx < result.size(); ++idx)
                result.d_data(idx) = RNG::uniform(lower, upper);
            return result;
        }

        // Creates a random matrix filled with N(mean, sd)
        [[nodiscard]] static Matrix randn(size_t const n, size_t const m, value_type const mean, value_type const sd)
            requires(IsDynamicMatrixData<MatrixData, value_type>)
        {
            Matrix result(n, m);
            for (size_t idx = 0; idx < n * m; ++idx)
                result.d_data(idx) = RNG::normal(mean, sd);
            return result;
        }

        [[nodiscard]] static Matrix randn(value_type const mean, value_type const sd)
            requires(!IsDynamicMatrixData<MatrixData, value_type>)
        {
            Matrix result;
            for (size_t idx = 0; idx < result.size(); ++idx)
                result.d_data(idx) = RNG::normal(mean, sd);
            return result;
        }

        [[nodiscard]] static Matrix diag(size_t const n, size_t const m, value_type const val)
            requires(IsDynamicMatrixData<MatrixData, value_type>)
        {
            Matrix result(n, m);
            for (size_t idx = 0; idx < std::min(n, m); ++idx)
                result(idx, idx) = val;
            return result;
        }
        [[nodiscard]] static Matrix diag(value_type const val)
            requires(!IsDynamicMatrixData<MatrixData, value_type>)
        {
            Matrix result;
            for (size_t idx = 0; idx < std::min(result.cols(), result.rows()); ++idx)
                result(idx, idx) = val;
            return result;
        }

        [[nodiscard]] static Matrix identity(size_t const n)
        {
            if constexpr (IsDynamicMatrixData<MatrixData, value_type>)
                return diag(n, n, static_cast<value_type>(1));
            else
                return diag(static_cast<value_type>(1));
        }

        // Generalized x(), y(), z(), and a() methods
        value_type x() const
            requires std::is_same_v<MatrixData, StaticMatrixData<1, 1, value_type>>
        {
            return d_data(0);
        }
        value_type x() const
            requires std::is_same_v<MatrixData, StaticMatrixData<2, 1, value_type>>
        {
            return d_data(0);
        }
        value_type x() const
            requires std::is_same_v<MatrixData, StaticMatrixData<3, 1, value_type>>
        {
            return d_data(0);
        }
        value_type x() const
            requires std::is_same_v<MatrixData, StaticMatrixData<4, 1, value_type>>
        {
            return d_data(0);
        }

        value_type y() const
            requires std::is_same_v<MatrixData, StaticMatrixData<2, 1, value_type>>
        {
            return d_data(1);
        }
        value_type y() const
            requires std::is_same_v<MatrixData, StaticMatrixData<3, 1, value_type>>
        {
            return d_data(1);
        }
        value_type y() const
            requires std::is_same_v<MatrixData, StaticMatrixData<4, 1, value_type>>
        {
            return d_data(1);
        }

        value_type z() const
            requires std::is_same_v<MatrixData, StaticMatrixData<3, 1, value_type>>
        {
            return d_data(2);
        }
        value_type z() const
            requires std::is_same_v<MatrixData, StaticMatrixData<4, 1, value_type>>
        {
            return d_data(2);
        }

        value_type a() const
            requires std::is_same_v<MatrixData, StaticMatrixData<4, 1, value_type>>
        {
            return d_data(3);
        }

        value_type &x()
            requires std::is_same_v<MatrixData, StaticMatrixData<1, 1, value_type>>
        {
            return d_data(0);
        }
        value_type &x()
            requires std::is_same_v<MatrixData, StaticMatrixData<2, 1, value_type>>
        {
            return d_data(0);
        }
        value_type &x()
            requires std::is_same_v<MatrixData, StaticMatrixData<3, 1, value_type>>
        {
            return d_data(0);
        }
        value_type &x()
            requires std::is_same_v<MatrixData, StaticMatrixData<4, 1, value_type>>
        {
            return d_data(0);
        }

        value_type &y()
            requires std::is_same_v<MatrixData, StaticMatrixData<2, 1, value_type>>
        {
            return d_data(1);
        }
        value_type &y()
            requires std::is_same_v<MatrixData, StaticMatrixData<3, 1, value_type>>
        {
            return d_data(1);
        }
        value_type &y()
            requires std::is_same_v<MatrixData, StaticMatrixData<4, 1, value_type>>
        {
            return d_data(1);
        }

        value_type &z()
            requires std::is_same_v<MatrixData, StaticMatrixData<3, 1, value_type>>
        {
            return d_data(2);
        }
        value_type &z()
            requires std::is_same_v<MatrixData, StaticMatrixData<4, 1, value_type>>
        {
            return d_data(2);
        }

        value_type &a()
            requires std::is_same_v<MatrixData, StaticMatrixData<4, 1, value_type>>
        {
            return d_data(3);
        }
    };

    template <size_t n, size_t m>
    using Mat = Matrix<float, StaticMatrixData<n, m, float>>;
    using Mat2x2 = Matrix<float, StaticMatrixData<2, 2, float>>;
    using Mat3x3 = Matrix<float, StaticMatrixData<3, 3, float>>;
    using Mat4x4 = Matrix<float, StaticMatrixData<4, 4, float>>;
    using Matrixf = Matrix<float, DynamicMatrixData<float>>;
    using Matrixd = Matrix<double, DynamicMatrixData<double>>;
    using Matrixi = Matrix<int, DynamicMatrixData<int>>;

    // Special Matrices. (Rotation, Random, Identity, Diagonal, all, etc.)
    using Vec4f = Matrix<float, StaticMatrixData<4, 1, float>>;
    using Vec3f = Matrix<float, StaticMatrixData<3, 1, float>>;
    using Vec2f = Matrix<float, StaticMatrixData<2, 1, float>>;

    template <typename scalar>
    using Vec2 = Matrix<scalar, StaticMatrixData<2, 1, scalar>>;
    template <typename scalar>
    using Vec3 = Matrix<scalar, StaticMatrixData<3, 1, scalar>>;
    template <typename scalar>
    using Vec4 = Matrix<scalar, StaticMatrixData<4, 1, scalar>>;

    template <size_t n>
    using Vecf = Matrix<float, StaticMatrixData<n, 1, float>>;

}

#endif
