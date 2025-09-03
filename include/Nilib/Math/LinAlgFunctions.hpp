#ifndef _LINALG_FUNCTIONS_H
#define _LINALG_FUNCTIONS_H

#include <format>

#include "Nilib/Math/LinAlg.hpp"

namespace Nilib
{
    // Linear Algebra Math functions.

    // Add two matrices.
    template <typename scalar, typename data>
    Matrix<scalar, data> operator+(Matrix<scalar, data> const &A, Matrix<scalar, data> const &B)
    {
        ASSERT(A.rows() == B.rows() && A.cols() == B.cols(), A.rows(), "!=", B.rows(), "or", A.cols(), "!=", B.cols());
        Matrix<scalar, data> result(A);
        result += B;
        return result;
    }

    // Subtract two matrices.
    template <typename scalar, typename data>
    Matrix<scalar, data> operator-(Matrix<scalar, data> const &A, Matrix<scalar, data> const &B)
    {
        ASSERT(A.rows() == B.rows() && A.cols() == B.cols(), A.rows(), "!=", B.rows(), "or", A.cols(), "!=", B.cols());
        Matrix<scalar, data> result(A);
        result -= B;
        return result;
    }

    // Multiplication/Division by scalar.
    template <typename scalar, typename data>
    Matrix<scalar, data> operator*(Matrix<scalar, data> const &A, scalar const lambda)
    {
        Matrix<scalar, data> result(A);
        result *= lambda;
        return result;
    }
    template <typename scalar, typename data>
    Matrix<scalar, data> operator*(scalar const lambda, Matrix<scalar, data> const &A) { return operator*(A, lambda); }
    template <typename data, typename scalar>
    Matrix<scalar, data> operator/(Matrix<scalar, data> const &A, scalar const lambda) { return operator*(A, static_cast<scalar>(1.0 / lambda)); }
    template <typename data, typename scalar>
    Matrix<scalar, data> operator/(scalar const lambda, Matrix<scalar, data> const &A) { return operator*(A, static_cast<scalar>(1.0 / lambda)); }

    // DynMatrix DynMatrix Product.
    template <typename type>
    Matrix<type, DynamicMatrixData<type>>
    operator*(Matrix<type, DynamicMatrixData<type>> const &A,
              Matrix<type, DynamicMatrixData<type>> const &B)
    {
        ASSERT(A.cols() == B.rows(), std::format("A:{}x{} B:{}x{}", A.rows(), A.cols(), B.rows(), B.cols()));

        Matrix<type, DynamicMatrixData<type>> res(A.rows(), B.cols());
        for (size_t nridx = 0; nridx < A.rows(); ++nridx)
            for (size_t k = 0; k < A.cols(); ++k)
                for (size_t ncidx = 0; ncidx < B.cols(); ++ncidx)
                    res(nridx, ncidx) += A(nridx, k) * B(k, ncidx);
        return res;
    }

    // StatMatrix StatMatrix Product.
    template <size_t n, size_t m, size_t p, typename type>
    Matrix<type, StaticMatrixData<n, p, type>>
    operator*(Matrix<type, StaticMatrixData<n, m, type>> const &A, Matrix<type, StaticMatrixData<m, p, type>> const &B)
    {
        Matrix<type, StaticMatrixData<n, p, type>> res(A.rows(), B.cols());
        for (size_t nridx = 0; nridx < A.rows(); ++nridx)
            for (size_t k = 0; k < A.cols(); ++k)
                for (size_t ncidx = 0; ncidx < B.cols(); ++ncidx)
                    res(nridx, ncidx) += A(nridx, k) * B(k, ncidx);
        return res;
    }

    // StatMatrix DynMatrix Product.
    template <size_t n, size_t m, typename type>
    Matrix<type, DynamicMatrixData<type>> operator*(Matrix<type, StaticMatrixData<n, m, type>> const &A, Matrix<type, DynamicMatrixData<type>> const &B)
    {
        ASSERT(A.cols() == B.rows(), std::format("A:{}x{} B:{}x{}", A.rows(), A.cols(), B.rows(), B.cols()));
        Matrix<type, DynamicMatrixData<type>> res(A.rows(), B.cols());
        for (size_t nridx = 0; nridx < A.rows(); ++nridx)
            for (size_t k = 0; k < A.cols(); ++k)
                for (size_t ncidx = 0; ncidx < B.cols(); ++ncidx)
                    res(nridx, ncidx) += A(nridx, k) * B(k, ncidx);
        return res;
    }

    // DynMatrix StatMatrix Product.
    template <size_t n, size_t m, typename type>
    Matrix<type, DynamicMatrixData<type>> operator*(Matrix<type, DynamicMatrixData<type>> const &A, Matrix<type, StaticMatrixData<n, m, type>> const &B)
    {
        ASSERT(A.cols() == B.rows(), std::format("A:{}x{} B:{}x{}", A.rows(), A.cols(), B.rows(), B.cols()));
        Matrix<type, DynamicMatrixData<type>> res(A.rows(), B.cols());
        for (size_t nridx = 0; nridx < A.rows(); ++nridx)
            for (size_t k = 0; k < A.cols(); ++k)
                for (size_t ncidx = 0; ncidx < B.cols(); ++ncidx)
                    res(nridx, ncidx) += A(nridx, k) * B(k, ncidx);
        return res;
    }

    // SparseMatrix SparseMatrix Product.
    template <typename type>
    Matrix<type, SparseMatrixData<type>>
    operator*(Matrix<type, SparseMatrixData<type>> const &A,
              Matrix<type, SparseMatrixData<type>> const &B)
    {
        ASSERT(A.cols() == B.rows(), std::format("A:{}x{} B:{}x{}", A.rows(), A.cols(), B.rows(), B.cols()));
        Matrix<type, SparseMatrixData<type>> res(A.rows(), B.cols());
        for (size_t nridx = 0; nridx < A.rows(); ++nridx)
            for (size_t k = 0; k < A.cols(); ++k)
                for (size_t ncidx = 0; ncidx < B.cols(); ++ncidx)
                    res(nridx, ncidx) += A(nridx, k) * B(k, ncidx);
        return res;
    }

    template <typename value_type, typename storage>
    Matrix<value_type, storage> transpose(Matrix<value_type, storage> const &A)
    {
        Matrix<value_type, storage> result(A.cols(), A.rows());
        for (size_t ridx = 0; ridx < A.cols(); ++ridx)
            for (size_t cidx = 0; cidx < A.rows(); ++cidx)
                result(ridx, cidx) = A(cidx, ridx);
        return result;
    }

    // Hadamar (element wise)
    // Specialization.
    template <typename data>
    Matrix<data> hadamar(Matrix<data> const &A, Matrix<data> const &B)
    {
        ASSERT(A.rows() == B.rows(), std::format("{}x{} != {}x{}", A.rows(), A.cols(), B.rows(), B.cols()));
        ASSERT(A.cols() == B.cols(), std::format("{}x{} != {}x{}", A.rows(), A.cols(), B.rows(), B.cols()));
        Matrix<data> res(A.rows(), B.cols());

        for (size_t ridx = 0; ridx < A.rows(); ++ridx)
            for (size_t cidx = 0; cidx < A.cols(); ++cidx)
                res(ridx, cidx) = A(ridx, cidx) * B(ridx, cidx);
        return res;
    }

    // Cbind.
    template <size_t n, size_t m, size_t p, typename type>
    Matrix<type, StaticMatrixData<n + p, m, type>> cbind(Matrix<type, StaticMatrixData<n, m, type>> const &A, Matrix<type, StaticMatrixData<p, m, type>> const &B)
    {
        Matrix<type, StaticMatrixData<n + p, m, type>> res;
        for (size_t nridx = 0; nridx < A.rows() + B.rows(); ++nridx)
            for (size_t ncidx = 0; ncidx < A.cols(); ++ncidx)
                res(nridx, ncidx) = (nridx < A.rows()) ? A(nridx, ncidx) : B(nridx - A.rows(), ncidx);
        return res;
    }
    template <typename type>
    Matrix<type, DynamicMatrixData<type>> cbind(Matrix<type, DynamicMatrixData<type>> const &A, Matrix<type, DynamicMatrixData<type>> const &B)
    {
        CORE_ASSERT(A.cols() == B.cols());
        Matrix<type, DynamicMatrixData<type>> res(A.rows() + B.rows(), B.cols());
        for (size_t nridx = 0; nridx < A.rows() + B.rows(); ++nridx)
            for (size_t ncidx = 0; ncidx < A.cols(); ++ncidx)
                res(nridx, ncidx) = (nridx < A.rows()) ? A(nridx, ncidx) : B(nridx - A.rows(), ncidx);
        return res;
    }
    // Rbind.
    template <size_t n, size_t m, size_t p, typename type>
    Matrix<type, StaticMatrixData<n + p, m, type>> rbind(Matrix<type, StaticMatrixData<n, m, type>> const &A, Matrix<type, StaticMatrixData<p, m, type>> const &B)
    {
        CORE_ASSERT(A.rows() == B.rows());
        Matrix<type, StaticMatrixData<n + p, m, type>> res;
        for (size_t nridx = 0; nridx < A.rows(); ++nridx)
            for (size_t ncidx = 0; ncidx < A.cols() + B.cols(); ++ncidx)
                res(nridx, ncidx) = (ncidx < A.cols()) ? A(nridx, ncidx) : B(nridx, ncidx - A.cols());
        return res;
    }
    template <typename type>
    Matrix<type, DynamicMatrixData<type>> rbind(Matrix<type, DynamicMatrixData<type>> const &A, Matrix<type, DynamicMatrixData<type>> const &B)
    {
        CORE_ASSERT(A.rows() == B.rows());
        Matrix<type, DynamicMatrixData<type>> res(A.rows(), A.cols() + B.cols());
        for (size_t nridx = 0; nridx < A.rows(); ++nridx)
            for (size_t ncidx = 0; ncidx < A.cols() + B.cols(); ++ncidx)
                res(nridx, ncidx) = (ncidx < A.cols()) ? A(nridx, ncidx) : B(nridx, ncidx - A.cols());
        return res;
    }

    template <typename value_type, typename data, typename Callable>
    Matrix<value_type, data> apply(Matrix<value_type, data> const &A, Callable &&fun)
    {
        Matrix<value_type, data> tmp = A;
        tmp.apply(std::forward<Callable>(fun));
        return tmp;
    }

    // Distance functions between Matrices/Vectors.
    template <typename type, typename storage, float p>
    type minkowski(Matrix<type, storage> const &A, Matrix<type, storage> const &B)
    {
        return std::pow(apply(A - B, [](type t)
                              { return std::pow(std::abs(t), p); })
                            .sum(),
                        1 / p);
    }

    template <typename type = float, typename storage = DynamicMatrixData<type>>
    type euclidean(Matrix<type, storage> const &A, Matrix<type, storage> const &B)
    {
        return minkowski<type, 2.0f>(A, B);
    }

    template <typename type>
    type euclidean(Vec2<type> const &A, Vec2<type> const &B)
    {
        return std::sqrt(euclidean2(A, B));
    }
    // Specializations for Vec2
    template <typename type>
    type euclidean2(Vec2<type> const &A, Vec2<type> const &B)
    {
        // Access the underlying data (assuming a 2x1 static matrix structure)
        type diff1 = A(0) - B(0);
        type diff2 = A(1) - B(1);

        // Compute squared Euclidean distance directly
        return diff1 * diff1 + diff2 * diff2;
    }
    // Specializations for Vec3
    template <typename type>
    type euclidean2(Vec3<type> const &A, Vec3<type> const &B)
    {
        // Access the underlying data (assuming a 3x1 static matrix structure)
        type diff1 = A(0) - B(0);
        type diff2 = A(1) - B(1);
        type diff3 = A(2) - B(2);

        // Compute squared Euclidean distance directly
        return diff1 * diff1 + diff2 * diff2 + diff3 * diff3;
    }

    template <typename type, typename storage>
    type manhattan(Matrix<type, storage> const &A, Matrix<type, storage> const &B)
    {
        return apply(A - B, [](type t)
                     { return std::abs(t); })
            .sum();
    }

    // Specialization for Vec2.
    template <typename type>
    type manhattan(Vec2<type> const &A, Vec2<type> const &B)
    {
        // Access the underlying data (assuming a 2x1 static matrix structure)
        type diff1 = A(0, 0) - B(0, 0);
        type diff2 = A(1, 0) - B(1, 0);

        // Compute Manhattan distance directly
        return std::abs(diff1) + std::abs(diff2);
    }

    // Normalize matrix as if it were a vector.
    template <typename value_type, typename type>
    Matrix<value_type, type> normalize(Matrix<value_type, type> const &A)
    {
        auto tmp = A;
        return tmp / A.sum();
    }
    // Normalize vector.
    template <typename type>
    Matrix<type> normalize(Matrix<type>  &A)
    {
        return A / A.sum();
    }

    template <typename value_type, typename type>
    void normalize(Matrix<value_type, type> &A)
    {
        return A / A.sum();
    }

    template <typename value_type, typename type>
    Matrix<value_type, type> rownormalize(Matrix<value_type, type> &A)
    {
        for (size_t rdx = 0; rdx < A.rows(); ++rdx)
        {
            value_type rowsum = 0.0f;
            for (size_t cdx = 0; cdx < A.cols(); ++cdx)
            {
                rowsum += A(rdx, cdx);
            }
            if (rowsum == 0.0f)
                continue;
            for (size_t cdx = 0; cdx < A.cols(); ++cdx)
            {
                A(rdx, cdx) /= rowsum;
            }
        }
        return A;
    }


}
#endif