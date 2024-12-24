#ifndef _MATRIX_ALG_H
#define _MATRIX_ALG_H

#include <ostream>
#include <sstream>
#include "Nilib/Math/MatrixStorage.hpp"
#include "Nilib/Logger/Log.hpp"
#include "Nilib/Core/Assert.hpp"

#include "Nilib/Math/RNG.hpp"


namespace Nilib {

    template <typename MatrixData>
    class Matrix
    {
        MatrixData d_data;

    public:
        auto operator()(size_t const row, size_t const col) const { return d_data(row, col); }
        auto &operator()(size_t const row, size_t const col) { return d_data(row, col); }

        size_t rows() const {return d_data.rows(); }
        size_t cols() const {return d_data.cols(); }

        // Inserts the matrix into a stream. 
        
        friend std::ostream &operator<<(std::ostream &os, Matrix const &mat) {
            if constexpr (std::is_same_v<MatrixData, DynamicMatrixData<float>>)
                return os << "<Matrixf " << mat.d_data.rows() << 'x' << mat.d_data.cols() << '>';
            else if constexpr (std::is_same_v<MatrixData, DynamicMatrixData<double>>)
                return os << "<Matrixd " << mat.d_data.rows() << 'x' << mat.d_data.cols() << '>';
            else if constexpr (std::is_same_v<MatrixData, DynamicMatrixData<decltype(d_data.sum())>>)
                return os << "<Matrix " << mat.d_data.rows() << 'x' << mat.d_data.cols() << '>';
            else
                return os << "<Mat " << mat.d_data.rows() << 'x' << mat.d_data.cols() << '>';
        }
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

        // Returns the sum of all the entries in the Matrix. 
        auto sum() const { return d_data.sum(); }
        auto apply(std::function<float(float)> fun) { return d_data.apply(fun); }

        Matrix() = default;
        Matrix(size_t const rows, size_t const cols)
         : d_data(rows, cols) {}
        Matrix(size_t const rows, size_t const cols, MatrixData const &data)
         : d_data(rows, cols) {
            d_data(data);
         }
        Matrix(MatrixData const &data) 
        :   d_data(data) { }


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

        // // In place transpose (only available for dynamic storage).
        // template <typename type>
        // std::enable_if<std::is_same<U, DynamicMatrixData<type>>::value>
        // void T() const {

        // }


        // static constructors.
        static Matrix all(float const val) {
            Matrix result;
            for (size_t idx = 0; idx < result.cols() * result.rows(); idx++)
                result.d_data(idx) = val;
            return result;
        }

        // static constructors.
        static Matrix all(size_t const n, size_t const m, float const val) {
            Matrix result(n, m);
            for (size_t idx = 0; idx < result.cols() * result.rows(); idx++)
                result.d_data(idx) = val;
            return result;
        }
        
        static Matrix zeros() {
            return Matrix::all(0.0);
        }

        static Matrix ones() {
            return Matrix::all(1.0);
        }

        // Creates a random matrix filled with [0-1]. 
        //requires (MatrixData == decltype(DynamicMatrixData))
        static Matrix rand(size_t const n, size_t const m) {
            Matrix result(n, m);
            for (size_t idx = 0; idx < n * m; ++idx)
                    result.d_data(idx) = RNG::uniform(0.0, 1.0);
            return result;
        }
        static Matrix rand() {
            Matrix result;
            for (size_t idx = 0; idx < result.rows() * result.cols(); ++idx)
                result.d_data(idx) = RNG::uniform(0.0, 1.0);
            return result;    
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

    template<size_t n, size_t m> using Mat = Matrix<StaticMatrixData<n,m,float>>;
    using Mat3x3 = Matrix<StaticMatrixData<3, 3, float>>;
    using Matrixf = Matrix<DynamicMatrixData<float>>;
    using Matrixd = Matrix<DynamicMatrixData<double>>;

    // Special Matrices. (Rotation, Random, Identity, Diagonal, all, etc.)



}

#endif

