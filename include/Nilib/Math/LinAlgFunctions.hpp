#ifndef _LINALG_FUNCTIONS_H
#define _LINALG_FUNCTIONS_H

#include "Nilib/Math/LinAlg.hpp"

namespace Nilib {
    // Linear Algebra Math functions.

    // Add two matrices. 
    template<typename data>
    Matrix<data> operator+(Matrix<data> const &A, Matrix<data> const &B)
    {
        Matrix<data> result(A);
        result += B;
        return result;
    }
    // Subtract two matrices. 
    template<typename data>
    Matrix<data> operator-(Matrix<data> const &A, Matrix<data> const &B) { return operator+(A, -1.0 * B);}

    // Multiplication/Division by scalar. 
    template<typename data, typename scalar>
    Matrix<data> operator*(Matrix<data> const &A, scalar const lambda)
    {
        Matrix<data> result(A);
        result *= lambda;
        return result;
    } 
    template<typename data, typename scalar>
    Matrix<data> operator*(scalar const lambda, Matrix<data> const &A) { return operator*(A, lambda); }
    template<typename data, typename scalar>
    Matrix<data> operator/(Matrix<data> const &A, scalar const lambda) { return operator*(A, 1.0 / lambda); }
    template<typename data, typename scalar>
    Matrix<data> operator/(scalar const lambda, Matrix<data> const &A) { return operator*(A, 1.0 / lambda); }

    // Matrix Matrix Product.
    template<typename type>
    Matrix<DynamicMatrixData<type>> 
    operator*(Matrix<DynamicMatrixData<type>> const &A, 
              Matrix<DynamicMatrixData<type>> const &B) {
        CORE_ASSERT(A.cols() == B.rows());
        
        Matrix<DynamicMatrixData<type>> res(A.rows(), B.cols());
        for (size_t nridx = 0; nridx < A.rows(); ++nridx)
            for (size_t ncidx = 0; ncidx < B.cols(); ++ncidx)
                for (size_t k = 0; k < A.cols(); ++k)
                    res(nridx, ncidx) += A(nridx, k) * B(k, ncidx);
        return res;        
    }
    // Specialization.
    template<size_t n, size_t m, size_t p, typename type>
    Matrix<StaticMatrixData<n,p,type>> 
    operator*(Matrix<StaticMatrixData<n,m,type>> const &A, Matrix<StaticMatrixData<m,p,type>> const &B) {
        CORE_ASSERT(A.cols() == B.rows());
        
        Matrix<StaticMatrixData<n,p,type>> res(A.rows(), B.cols());
        for (size_t nridx = 0; nridx < A.rows(); ++nridx)
            for (size_t ncidx = 0; ncidx < B.cols(); ++ncidx)
                for (size_t k = 0; k < A.cols(); ++k)
                    res(nridx, ncidx) += A(nridx, k) * B(k, ncidx);
        return res;        
    }

    // // Matrix Matrix Matrix.    '
    // template<typename type>
    // Matrix<DynamicMatrixData<type>> 
    // times(Matrix<DynamicMatrixData<type>> const &lhs, 
    //       Matrix<DynamicMatrixData<type>> const &mid,
    //       Matrix<DynamicMatrixData<type>> const &rhs) {
    //     Matrix<DynamicMatrixData<type>> res = lhs * mid * rhs;
    //     return res;        
    // }
    // // Matrix Matrix Matrix.    '
    // template<size_t n, size_t m, size_t p, size_t k, typename type>
    // Matrix<StaticMatrixData<n,k,type>> 
    // times(Matrix<StaticMatrixData<n,m,type>> const &lhs, 
    //           Matrix<StaticMatrixData<m,p,type>> const &mid,
    //           Matrix<StaticMatrixData<p,k,type>> const &rhs) {
    //     Matrix<StaticMatrixData<n, k, type>> res = (lhs * mid) * rhs;
    //     return res;        
    // }

    template<typename data>
    Matrix<data> transpose(Matrix<data> const &A) {
        Matrix<data> result(A.cols(), A.rows());
        for (size_t ridx = 0; ridx < A.cols(); ++ridx)
            for (size_t cidx = 0; cidx < A.rows(); ++cidx)
                result(ridx, cidx) = A(cidx, ridx);
        return result;
    }

    // Hadamar (element wise)
        // Specialization.
    template<typename data>
    Matrix<data> hadamar(Matrix<data> const &A, Matrix<data> const &B) {
        CORE_ASSERT(A.rows() == B.rows());
        CORE_ASSERT(A.cols() == B.cols());
        Matrix<data> res(A.rows(), B.cols());

        for (size_t nridx = 0; nridx < A.rows() * A.cols(); ++nridx)
            res.d_data(nridx) = A.d_data(nridx) * B.d_data(nridx);
        return res;        
    }

}

#endif