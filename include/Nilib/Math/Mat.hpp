#ifndef _LINALG_H
#define _LINALG_H

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Core/Assert.hpp"
#include "Nilib/Math/RNG.hpp"

#include <array>
#include <iostream>
#include <cmath>
#include <functional>


// Static Matrix class.
template<size_t n = 1, size_t m = 1, typename Type = float>
class Mat
{
    
private:
    std::array<Type, n * m> d_data;

public:
    Mat() 
    {
    };

    Mat(Type (&arr)[n*m]) {
        d_data = std::to_array<Type>(arr);
    }
    Mat(Type (&&arr)[n*m]) {
        d_data = std::to_array<Type>(arr);
    }

    // Acces operators. 
    Type &operator()(size_t const row, size_t const col) {
        return d_data[row * m + col];
    }
    
    Type operator()(size_t const row, size_t const col) const {
        return d_data[row * m + col];
    }

    // Create a diagonal matrix. 
    Mat static diag(Type const value) {
        Mat res;
        res.d_data.fill(0);
        for (size_t i = 0; i < std::min(n,m); i++)
            res(i,i) = value;        
        return res;
    }
    // Create an identity matrix. 
    Mat static identity() requires (n == m) { return diag(1); }
    
    Mat &operator*=(Type const value){
        *this = operator*(*this, value);
        return *this;
    }
    Mat &operator/=(Type const value){
        *this = operator*(*this,1 / value);
        return *this;
    }

    Mat &operator+=(Mat const &B){
        *this = operator+(*this, B);
        return *this;
    }

    Mat &operator-=(Mat const &B){
        *this = operator-(*this, B);
        return *this;
    }

    friend Mat apply(Mat const &A, std::function<Type(Type)> apply_fun)
    {
        Mat res;
        for (size_t row = 0; row < n; ++row)
            for (size_t col = 0; col < m; ++col)
                res.operator()(row, col) = apply_fun(A.operator()(row, col));
        return res;
    }

    // Swap rows. 
    void swaprows(size_t const R1, size_t const R2){

    }
    // Multiply row by value. 
    void multiplyrow(size_t const R, Type const value) {

    }
    // Add rows R1 + R2.
    void plusrows(size_t const R1, size_t const R2) {

    }

    // Returns the sum of entries. 
    Type sum() const {
        Type sum = 0;
        for (size_t row = 0; row < n; ++row)
            for (size_t col = 0; col < m; ++col)
                sum += this->operator()(row, col);
        return sum;
    }

    Type magnitude() const requires(m == 1) {
        Type res = dot(*this, *this);
        //ASSERT(std::sqrt(res) < 10e-20, "Magnitude (near)zero!\n");
        return std::sqrt(res);
    }

    // Friends.
    friend Type angle(Mat const &a, Mat const &b) requires (m == 1) {
        CORE_ASSERT(a.magnitude() != 0)
        CORE_ASSERT(b.magnitude() != 0)
        return dot(a, b) / (a.magnitude() * b.magnitude());        
    }

    // Element wise multiplication. 
    friend Type dot(Mat const &a, Mat const &b) {
        Type res = 0; 
        for (size_t row = 0; row < n; ++row)
            for (size_t col = 0; col < m; ++col)
                res += a.operator()(row, col) * b.operator()(row, col);
        return res;
    }

    // Add two Matrices of compatible dimensions. 
    friend Mat operator+(Mat const &A, Mat const &B) {
        Mat res;
        for (size_t row = 0; row < n; ++row)
            for (size_t col = 0; col < m; ++col)
                res(row, col) = A(row, col) + B(row, col);
        return res;
    }

    // Subtract two Matrices of compatible dimensions.
    friend Mat operator-(Mat const &A, Mat const &B) {
        return operator+(A, operator*(B, -1));
    }

    // Multiply by a value. 
    friend Mat operator*(Mat const &A, Type const value) {
        Mat res;
        for (size_t row = 0; row < n; ++row)
            for (size_t col = 0; col < m; ++col)
                res(row, col) = A(row,  col) * value;
        return res;
    }

    friend Mat operator*(Type const value, Mat const &A) {
        return operator*(A, value);
    }

    // Multiply two matrices of compatible dimensions.
    template<size_t k>
    friend Mat<n, k, Type> operator*(Mat<n, m, Type> const &A, Mat<m, k, Type>  const &B) {
        Mat<n, k, Type> res;
        for (size_t row = 0; row < n; ++row)
            for (size_t col = 0; col < k; ++col)
            {
                Type sum = 0;
                for (size_t j = 0; j < m; ++j)
                    sum += A(row, j) * B(j, col); 
                res(row, col) = sum;
            }
        return res;
    }
    

    // Compactly display in stream.
    friend std::ostream &operator<<(std::ostream &os, Mat const &mat) {
        if constexpr (m > 1)
            os << "<Mat" << n << 'x' << m << '>';
        else
            os << "<Vec" << n << "D>";  
        for (size_t row = 0; row < n; ++row)
        {
            os << '<';
            for (size_t col = 0; col < m - 1; ++col)
                os << mat(row, col) << ',';
            os << mat(row, m - 1) << '>';
        }
        return os;
    }

    void rand() {
        for (size_t row = 0; row < n; ++row)
            for (size_t col = 0; col < m; ++col)
                this->operator()(row,col) = RNG::uniform<Type>(0, 1);
    }

    void randn(Type const mean, Type const std_dev) {
        for (size_t row = 0; row < n; ++row)
            for (size_t col = 0; col < m; ++col)
                this->operator()(row,col) = RNG::normal<Type>(mean, std_dev);
    }

    // Random matrix with entries 0-1.
    Mat static create_rand() {
        Mat res;
        res.rand();
        return res;
    }

    // Random matrix with all equal to value.
    Mat static all(Type const value) {
        Mat res;
        res.d_data.fill(value);
        return res;
    }
    Mat static create_ones() { return Mat::all(1); }
    Mat static create_zeros() { return Mat::all(0); }
    Mat static create_fill(Type const t) { return Mat::all(t); }

    void fill(Type const value) {std::fill(d_data.begin(), d_data.end(), value);}
    void ones() { return this->fill(1); }
    void zeros() { return this->fill(0); }

    // Random matrix with entries N(0, 1).
    Mat static create_randn(Type const mean, Type const std_dev) {
        Mat res;
        res.randn(mean, std_dev);
        return res;
    }

    
    // Modify and access.
    Type x() const requires (n >= 1 and m == 1) {return this->d_data[0]; } ; 
    Type &x() requires (n >= 1 and m == 1) {return this->d_data[0]; }  
    Type y() const requires (n >= 2 and m == 1) {return this->d_data[1]; }  
    Type &y() requires (n >= 2 and m == 1) {return this->d_data[1]; }  
    Type z() const requires (n >= 3 and m == 1) {return this->d_data[2]; }  
    Type &z() requires (n >= 3 and m == 1) {return this->d_data[2]; } 
    Type a() const requires (n >= 4 and m == 1) {return this->d_data[3]; }  
    Type &a() requires (n >= 4 and m == 1){return this->d_data[3]; } 
    
    Mat<2, 1, Type> xy() const requires (n >= 2 and m == 1) {return Mat<2, 1, Type>({this->d_data[0], this->d_data[1]});}  
    Mat<3, 1, Type> xyz() const requires (n >= 3 and m == 1) {return Mat<3, 1, Type>({this->d_data[0], this->d_data[1], this->d_data[2]});}
    
    Type const * get() const {return d_data.data();}; 
};

template<size_t n, typename Type>
using Vec = Mat<n, 1, Type>;
template<typename Type>
using Vec2D = Mat<2, 1, Type>;
template<typename Type>
using Vec3D = Mat<3, 1, Type>;
template<typename Type>
using Vec4D = Mat<4, 1, Type>;

template<typename Type>
using Mat2x2 = Mat<2, 2, Type>;
template<typename Type>
using Mat3x3 = Mat<3, 3, Type>;
template<typename Type>
using Mat4x4 = Mat<4, 4, Type>;



// Transpose a matrix. 
template<typename Type, size_t n, size_t m>
Mat<m, n, Type> transpose(Mat<n, m, Type> const &A) {
    Mat<m, n, Type> res;
    for (size_t row = 0; row < m; ++row)
        for (size_t col = 0; col < n; ++col)
        {
            res(row, col) = A(col, row);
        }
    return res;
}

// Extend a matrix horizontally. 
template<typename Type, size_t n, size_t m, size_t k>
Mat<n, m + k, Type> cbind(Mat<n, m, Type> const &A, Mat<n, k, Type> const &B)
{
    Mat<n, m + k, Type> res;
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m + k; j++)
            res(i, j) = (j < m) ? A(i, j) : B(i,j-m);
    return res;
}
// Extend a matrix vertically. 
template<typename Type, size_t n, size_t m, size_t k>
Mat<n+k, m,Type> rbind(Mat<n, m, Type> const &A, Mat<k,m,Type> const &B)
{
    Mat<n+k, m, Type> res;
    for (size_t i = 0; i < n + k; i++)
        for (size_t j = 0; j < m; j++)
            res(i, j) = (i < n) ? A(i, j) : B(i - n,j);
    return res;
}

// Returns the determinant of square matrix A. 
template<typename Type, size_t n>
Type determinant(Mat<n, n, Type> const &A) {
    ASSERT(false, "Not yet implemented!");
}

// Returns the inverse of square matrix A.
template<typename Type, size_t n>
Mat<n, n, Type> inverse(Mat<n, n, Type> const &A) {
    ASSERT(false, "Not yet implemented!");
}


// Minkowski distance between A and B. 
template<typename Type, size_t n>
Type minkowski(Vec<n, Type> const &A, Vec<n, Type> const &B, Type const p)
{
    // Euclid.
    if (p == 2.0) { return sqrt((A.x() - B.x()) * (A.x() - B.x()) + (A.y() - B.y()) * (A.y() - B.y())); }
    
    // Manhattan.
    if (p == 1.0) { return apply(A - B, [](Type t){ return std::abs(t); }).sum(); }
    return std::pow(apply(A - B, [p](Type t){ return std::pow(std::abs(t), p); } ).sum(), 1 / p);
}

template<typename Type, size_t n>
Type manhattan(Vec<n, Type> const &A, Vec<n, Type> const &B)
{
    return minkowski(A, B, static_cast<Type>(1.0));
}

template<typename Type, size_t n>
Type euclidean(Vec<n, Type> const &A, Vec<n, Type> const &B)
{
    return minkowski(A, B, static_cast<Type>(2.0));
}

template<typename Type, size_t n>
Type euclidean2(Vec<n, Type> const &A, Vec<n, Type> const &B)
{
    return (A.x() - B.x()) * (A.x() - B.x()) + (A.y() - B.y()) * (A.y() - B.y());
}
#endif