#ifndef _LIN_ALG_H
#define _LIN_ALG_H

#include <ostream>
#include <sstream>
#include "Nilib/Math/MatrixStorage.hpp"
#include "Nilib/Logger/Log.hpp"
#include "Nilib/Core/Assert.hpp"

namespace Nilib {

    template <typename MatrixData>
    struct Matrix
    {
        MatrixData d_data;

    public:
        auto operator()(size_t const row, size_t const col) const { return d_data(row, col); }
        auto &operator()(size_t const row, size_t const col) { return d_data(row, col); }

        size_t rows() const {return d_data.rows(); }
        size_t cols() const {return d_data.cols(); }

        // Inserts the matrix into a stream. 
        friend std::ostream &operator<<(std::ostream &os, Matrix const &mat) {
            return os << "<Matrix " << mat.d_data.rows() << 'x' << mat.d_data.cols() << '>';
        }

        // Returns the sum of all the entries in the Matrix. 
        auto sum() const { return d_data.sum(); }

        Matrix() = default;
        Matrix(size_t const rows, size_t const cols)
         : d_data(rows, cols) {}
        Matrix(size_t const rows, size_t const cols, MatrixData const &data)
         : d_data(rows, cols) {
            d_data(data);
         }

        Matrix(MatrixData const &data) 
        :   d_data(data) { }

        // Pretty print the matrix.
        void print() const {
            for (size_t ridx = 0; ridx < d_data.rows(); ++ridx)
            {
                std::stringstream ss;
                ss << "(" << std::fixed << std::setprecision(3); 
                for (size_t cidx = 0; cidx < d_data.cols(); ++cidx)
                    ss << d_data(ridx,cidx) << ((cidx < d_data.cols()-1) ? ',' : ')');
                LOG_DEBUG(ss.str());
            }   
        }

        // In place operators.
        template<typename data>
        void operator+=(Matrix<data> const &B) {
            CORE_ASSERT(d_data.rows() == B.d_data.rows());
            CORE_ASSERT(d_data.cols() == B.d_data.cols());
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) += B.d_data(idx);
        }
        
        template<typename data>
        void operator-=(Matrix<data> const &B) {
            CORE_ASSERT(d_data.rows() == B.d_data.rows());
            CORE_ASSERT(d_data.cols() == B.d_data.cols());
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) -= B.d_data(idx);
        }

        template<typename scalar>
        void operator*=(scalar const lambda) {
            //d_data *= lambda; // Is defined for valarray.
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) *= lambda;
        }

        template<typename scalar>
        void operator/=(scalar const lambda) {
            for (size_t idx = 0; idx < d_data.size(); idx++)
                d_data(idx) /= lambda;
        }

        // // Multiply by other inplace Matrix. (Only possible when square)
        // template<typename data>
        // void operator*=(Matrix<data> const &B) {
        //     CORE_ASSERT(d_data.cols() == B.d_data.cols());
        //     CORE_ASSERT(d_data.rows() == B.d_data.rows());
        // }

        // In place transpose. (Only when dynamic storage)
        // void T() {
        //     for (size_t ridx = 0; ridx < rows(); ++ridx)
        //     {
        //         for (size_t cidx = 0; cidx < cols(); ++cidx)
        //         {
        //             d_data(ridx, cidx) = d_data(cidx, ridx);       
        //         }   
        //     }
        //     d_data.
        // }


        // static constructors. 
        static Matrix all(float const val) {
            Matrix result;
            for (size_t idx = 0; idx < result.d_data.size(); idx++)
                result.d_data(idx) = val;
            return result;
        }
        
        static Matrix zeros() {
            return Matrix::all(0.0);
        }

        static Matrix ones() {
            return Matrix::all(1.0);
        }

        static Matrix diag(size_t const n, size_t const m, float const val) {
            Matrix result(n, m);
            for (size_t idx = 0; idx < std::min(n, m); ++idx)
                result(idx, idx) = val;
            return result;
        }
        
        static Matrix identity(size_t const n) {
            return diag(n, n, 1.0);
        }

    };

    // Math.
    // Add two matrices. 
    template<typename data>
    Matrix<data> operator+(Matrix<data> const &A, Matrix<data> const &B)
    {
        Matrix<data> result(A.d_data);
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
    // Multiply by other Matrix. (Only possible when square)
    template<typename data, typename data2>
    Matrix<data> operator*(Matrix<data> const &A, Matrix<data2> const &B) {
        CORE_ASSERT(A.d_data.cols() == B.d_data.rows());
        
        Matrix<data> res(A.d_data.rows(), B.d_data.cols());
        for (size_t nridx = 0; nridx < A.d_data.rows(); ++nridx)
            for (size_t ncidx = 0; ncidx < B.d_data.cols(); ++ncidx)
                for (size_t k = 0; k < A.d_data.cols(); ++k)
                    res(nridx, ncidx) += A(nridx, k) * B(k, ncidx);
        return res;        
    }
    template<typename data>
    Matrix<data> transpose(Matrix<data> const &A) {
        Matrix<data> result(A.cols(), A.rows());
        for (size_t ridx = 0; ridx < A.cols(); ++ridx)
            for (size_t cidx = 0; cidx < A.rows(); ++cidx)
                result(ridx, cidx) = A(cidx, ridx);
        return result;
    }

    using Mat3x3 = Matrix<StaticMatrixData<3, 3, float>>;
    using Matrixf = Matrix<DynamicMatrixData<float>>;
    using Matrixd = Matrix<DynamicMatrixData<double>>;

    // Special Matrices. (Rotation, Random, Identity, Diagonal, all, etc.)



}

#endif

