#ifndef _MATRIX_DYNAMIC_H
#define _MATRIX_DYNAMIC_H

#include <vector> // For internal storage.
#include <functional> // For Matrix::apply()
#include <iostream> // For insertion into stream.
#include <cmath> // For sqrt

#include "Nilib/Core/Assert.hpp"
#include "Nilib/Math/RNG.hpp"
#include "Nilib/Core/Serializer.hpp"
#include "Nilib/Core/Deserializer.hpp"


// Matrix class.
template<typename Type>
class Matrix
{
    size_t n = 0;
    size_t m = 0;
    std::vector<Type> d_data{}; 
public:
    //Matrix() = default;

    Matrix(size_t n = 0, size_t m = 0)
     : Matrix(n, m, std::vector<Type>(n * m, 0))
    {
        //CORE_ASSERT(n > 0 && m > 0)
    }

    Matrix(size_t const n, size_t const m, std::vector<Type> const &data)
    : n{n}, m{m}, d_data{data}
    {
        //CORE_ASSERT(n > 0 && m > 0)
    }

    // Acces operators. 
    Type &operator()(size_t const row, size_t const col) {
        //CORE_ASSERT(row < n)
        //CORE_ASSERT(col < m)
        //CORE_ASSERT(row * m + col < d_data.size())
        return d_data[row * m + col];
    }
    Type operator()(size_t const row, size_t const col) const {
        //CORE_ASSERT(row < n)
        //CORE_ASSERT(col < m)
        //CORE_ASSERT(row * m + col < d_data.size())
        return d_data[row * m + col];
    }

    // Create a diagonal matrix. 
    Matrix static diag(size_t const n, size_t const m, Type const value) {
        Matrix res(n, m);
        for (size_t i = 0; i < std::min(n,m); i++)
            res(i,i) = value;        
        return res;
    }
    // Create an identity matrix. 
    Matrix static identity(size_t const n, size_t const m) {CORE_ASSERT(n == m); return diag(1); }
    
    Matrix &operator*=(Type const value){
        *this = operator*(*this, value);
        return *this;
    }
    Matrix &operator/=(Type const value){
        *this = operator*(*this, 1 / value);
        return *this;
    }

    Matrix &operator+=(Matrix const &B){
        *this = operator+(*this, B);
        return *this;
    }

    Matrix &operator-=(Matrix const &B){
        *this = operator-(*this, B);
        return *this;
    }

    bool operator==(Matrix const &other) const {
        return d_data == other.d_data;
    }

    template<typename T> friend Matrix<T> cbind(Matrix<T> const &A, Matrix<T> const &B);
    template<typename T> friend Matrix<T> transpose(Matrix<T> const &A);
    template<typename T> friend Matrix<T> hadamar(Matrix<T> const &A, Matrix<T> const &B);

    static Matrix apply(Matrix const &A, std::function<Type(Type)> apply_fun)
    {
        Matrix res(A.n, A.m);
        for (size_t row = 0; row < A.n; ++row)
            for (size_t col = 0; col < A.m; ++col)
                res.operator()(row, col) = apply_fun(A.operator()(row, col));
        return res;
    }

    
    void apply(std::function<Type(Type)> apply_fun)
    {
        for (size_t row = 0; row < n; ++row)
            for (size_t col = 0; col < m; ++col)
                this->operator()(row, col) = apply_fun(this->operator()(row, col));
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

    Type magnitude() {
        CORE_ASSERT(m == 1);
        Type res = dot(*this, *this);
        //ASSERT(std::sqrt(res) < 10e-20, "Magnitude (near)zero!\n");
        return std::sqrt(res);
    }

    // Friends.
    friend Type angle(Matrix const &a, Matrix const &b) {
        CORE_ASSERT(a.m == 1 && b.m == 1)
        CORE_ASSERT(a.magnitude() != 0)
        CORE_ASSERT(b.magnitude() != 0)
        return dot(a, b) / (a.magnitude() * b.magnitude());        
    }

    // Element wise multiplication and summation. 
    friend Type dot(Matrix const &a, Matrix const &b) {
        CORE_ASSERT(a.n == b.n && a.m == b.m)
        Type res = 0; 
        for (size_t row = 0; row < a.n; ++row)
            for (size_t col = 0; col < a.m; ++col)
                res += a.operator()(row, col) * b.operator()(row, col);
        return res;
    }

    // Add two Matrices of compatible dimensions. 
    friend Matrix operator+(Matrix const &A, Matrix const &B) {
        CORE_ASSERT(A.m == B.m && A.n == B.n)
        Matrix res(A.n, B.m);
        for (size_t row = 0; row < A.n; ++row)
            for (size_t col = 0; col < B.m; ++col)
                res(row, col) = A(row, col) + B(row, col);
        return res;
    }

    // Subtract two Matrices of compatible dimensions.
    friend Matrix operator-(Matrix const &A, Matrix const &B) {
        return operator+(A, operator*(B, -1));
    }

    // Multiply by a value. 
    friend Matrix operator*(Matrix const &A, Type const value) {
        Matrix res(A.n, A.m);
        for (size_t row = 0; row < A.n; ++row)
            for (size_t col = 0; col < A.m; ++col)
                res(row, col) = A(row,  col) * value;
        return res;
    }

    friend Matrix operator*(Type const value, Matrix const &A) {
        return operator*(A, value);
    }

    // Multiply two matrices of compatible dimensions.
    friend Matrix operator*(Matrix const &A, Matrix const &B) {
        CORE_ASSERT(A.m == B.n);
        Matrix res(A.n, B.m);
        for (size_t row = 0; row < A.n; ++row)
            for (size_t col = 0; col < B.m; ++col)
            {
                Type sum = 0;
                for (size_t j = 0; j < A.m; ++j)
                    sum += A(row, j) * B(j, col); 
                res(row, col) = sum;
            }
        return res;
    }
    

    // Compactly display in stream.
    friend std::ostream &operator<<(std::ostream &os, Matrix const &mat) {
        if (mat.m > 1)
            os << "<Mat" << mat.n << 'x' << mat.m << '>';
        else
            os << "<Vec" << mat.n << "D>";  
        for (size_t row = 0; row < mat.n; ++row)
        {
            os << '<';
            for (size_t col = 0; col < mat.m - 1; ++col)
                os << mat(row, col) << ',';
            os << mat(row, mat.m - 1) << '>';
        }
        return os;
    }

    void rand(size_t const n, size_t const m) {
        for (size_t row = 0; row < n; ++row)
            for (size_t col = 0; col < m; ++col)
                this->operator()(row,col) = RNG::uniform<Type>(0, 1);
    }

    void randn(Type const mean, Type const std_dev) {
        for (size_t row = 0; row < n; ++row)
            for (size_t col = 0; col < m; ++col)
                this->operator()(row,col) = RNG::normal<Type>(mean, std_dev);
    }

    // Random matrix with entries between 0-1.
    Matrix static create_rand(size_t const n, size_t const m) {
        Matrix res(n, m);
        res.rand(n, m);
        return res;
    }

    // Matrix filled with value.
    Matrix static all(size_t const n, size_t const m, Type const value) {
        Matrix res(n, m);
        std::fill(res.d_data.begin(), res.d_data.end(), value);
        return res;
    }
    Matrix static create_ones(size_t const n, size_t const m) { return Matrix::all(n, m, 1); }
    Matrix static create_zeros(size_t const n, size_t const m) { return Matrix::all(n, m, 0); }
    //Matrix static create_fill(Type const t) { return Matrix::all(t); }

    void fill(Type const value) {std::fill(d_data.begin(), d_data.end(), value);}
    void ones() { this->fill(1); }
    void zeros() { this->fill(0); }

    // Random matrix with entries N(0, 1).
    Matrix static create_randn(size_t const n, size_t const m, Type const mean, Type const std_dev) {
        Matrix res(n, m);
        res.randn(mean, std_dev);
        return res;
    }
    // Check validity of matrix. 
    bool containsNA()
    {
        for (auto &&num : d_data)
        {
            if (std::isnan(num)) return true;
        }
        return false;
    }
    Type const *data() const {
        return d_data.data();
    }
    Type *data() {
        return d_data.data();
    }

    explicit operator std::vector<Type>() {
        return d_data;
    }

    size_t cols() const { return m; }
    size_t rows() const { return n; }


    bool serialize(Serializer &serializer) {
        LOG_DEBUG() << "Serializing Matrix!\n";
        serializer.writeRaw(n);
        serializer.writeRaw(m);
        serializer.writeVector(d_data);
        LOG_SUCCESS("Serialized Matrix!\n");
        return true;
    }

    
    bool deserialize(Deserializer &deserializer) {
        LOG_DEBUG() << "Deserializing Matrix!\n";
        deserializer.readRaw(n);
        deserializer.readRaw(m);
        deserializer.readVector(d_data);
        LOG_DEBUG() << "Deserialized Matrix!\n";
        return true;
    }
};


// Element wise product.
template<typename Type> 
Matrix<Type> hadamar(Matrix<Type> const &A, Matrix<Type> const &B)
{
    CORE_ASSERT(A.n==B.n && A.m==B.m)
    Matrix<Type> res(A.n, A.m);
    for (size_t row = 0; row < A.n; ++row)
        for (size_t col = 0; col < A.m; ++col)
        {
            res(row, col) = A(row, col) * B(row, col);
        }
    return res;
}
// Transpose a matrix. 
template<typename Type>
Matrix<Type> transpose(Matrix<Type> const &A) {
    Matrix<Type> res(A.m, A.n);
    for (size_t row = 0; row < A.m; ++row)
        for (size_t col = 0; col < A.n; ++col)
        {
            res(row, col) = A(col, row);
        }
    return res;
}

// Extend a matrix horizontally. 
template<typename Type>
Matrix<Type> cbind(Matrix<Type> const &A, Matrix<Type> const &B)
{
    CORE_ASSERT(A.n == B.n)
    Matrix<Type> res(A.n, A.m + B.m);
    for (size_t i = 0; i < A.n; i++)
        for (size_t j = 0; j < A.m + B.m; j++)
            res(i, j) = (j < A.m) ? A(i, j) : B(i,j - A.m);
    return res;
}



// Sum over a matrix
template<typename Type>
Matrix<Type> colSums(Matrix<Type> const &B)
{ 
    Matrix<Type> res(1, B.cols());
    for (size_t i = 0; i < B.cols(); i++)
    {
        Type sum = 0;
        for (size_t j = 0; j < B.rows(); j++)
            sum += B(j, i);
        res(0, i) = sum;
    }
    return res;
}
// column means. 
template<typename Type>
Matrix<Type> colMeans(Matrix<Type> const &B)
{ 
    Matrix<Type> res(1, B.cols());
    for (size_t i = 0; i < B.cols(); i++)
    {
        Type sum = 0;
        for (size_t j = 0; j < B.rows(); j++)
            sum += B(j, i) / B.rows();
        res(0, i) = sum;
    }
    return res;
}

// Shorthand definitions.
using Matrixf = Matrix<float>;
using Matrixd = Matrix<double>;

#endif