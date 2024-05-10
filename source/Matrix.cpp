#include "Matrix.h"
#include "Log.h"

Matrix::Matrix(double *arr, size_t n, size_t m)
    : d_n(n), d_m(m), d_arr(new double[n * m]) {
    memcpy(static_cast<void*>(d_arr.get()), arr, n * m * sizeof(double));
    Log::debug("SingleCtor constructed a ") << d_n << " by " << d_m << " matrix.\n";
}


Matrix::Matrix(std::vector<double> arr, size_t n, size_t m)
    : Matrix(arr.data(), n, m) {
}

Matrix& Matrix::operator=(Matrix const &other) {
    if (this == &other) return *this; 
    if (d_n != other.d_n or d_m != other.d_m) throw std::range_error("Assignment: Matrices of incompatible dimension!");
    d_n = other.d_n;
    d_m = other.d_m;
    d_arr = other.d_arr;
    return *this;
}

Matrix::Matrix(Matrix const &other) {
    //if (d_n != other.d_n or d_m != other.d_m) throw std::range_error("Copy constructor: Matrices of incompatible dimension!");
    d_n = other.d_n;
    d_m = other.d_m;
    d_arr = other.d_arr;
}

Matrix::Matrix(Matrix const &&other) {
    if (d_n != other.d_n or d_m != other.d_m) throw std::range_error("Move constructor: Matrices of incompatible dimension!");
    d_n = other.d_n;
    d_m = other.d_m;
    d_arr = std::move(other.d_arr); // Overkill?
} 

Matrix Matrix::transpose() {
    // Construct a new matrix from the same array, but with switched dimensions. 
    return Matrix(static_cast<double*>(d_arr.get()), d_m, d_n); 
}

// Transpose in place. 
void Matrix::T(){
    std::swap(d_n, d_m);
}

// Element wise operators. 
bool Matrix::operator==(const Matrix& other) const {
    
    if (d_n != other.d_n or d_m != other.d_m) throw std::range_error("Matrices of incompatible dimension!");
    for (size_t idx = 0; idx < d_n * d_m; ++idx)
        if (d_arr[idx] != other[idx]) return false;
    return true;    
}

bool Matrix::operator>(const Matrix& other) const {
    if (d_n != other.d_n or d_m != other.d_m) throw std::range_error("Matrices of incompatible dimension!");
    for (size_t idx = 0; idx < d_n * d_m; ++idx)
        if (d_arr[idx] > other[idx]) return false;
    return true;   
}


std::ostream& operator<<(std::ostream& os, Matrix const &mat) {
    os << "<Matrix object " << mat.d_n << ' ' << mat.d_m << '[';
    for (size_t ridx = 0; ridx < mat.d_n; ++ridx)
    {
        os << '[';
        for (size_t cidx = 0; cidx < mat.d_m; ++cidx)
        {
            os << mat.d_arr.get()[ridx * mat.d_m + cidx] << ',';
        }
        os << "],";
    }
    os << "]>";
    return os;
}

Matrix Matrix::ones(size_t const n, size_t const m) {
    return Matrix(std::vector<double>(n * m, 1), n, m);
}

Matrix Matrix::zeros(size_t const n, size_t const m) {
    return Matrix(std::vector<double>(n * m, 0), n, m);
}

Matrix Matrix::random(size_t const n, size_t const m) {
    std::vector<double> randomValues;
    randomValues.reserve(n * m);
    
    static std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < n * m; ++i) {
        randomValues.push_back(dis(gen));
    }
    return Matrix(randomValues, n, m);
}


Vector::Vector(Matrix const &A)
    : Matrix(A)
{   
    d_n = d_n * d_m;
    d_m = 1;
    Log::info("Converted Matrix ") << A << " into Vector " << *this << '\n';
}

Vector::Vector(double *p, size_t n) 
    : Matrix(p, n, 1) {
}

Vector::Vector(std::vector<double> const vec) 
    : Matrix(vec, vec.size(), 1) {
    Log::debug("Constructed Vector object!") << *this << '\n';
}


Vector Vector::random(size_t n) {
    std::vector<double> randomValues;
    randomValues.reserve(n);
    
    static std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < n; ++i) {
        randomValues.emplace_back(dis(gen));
    }
    return Vector(randomValues);
}

Vector Vector::ones(size_t n) {
    return Vector(std::vector<double>(n, 1));
}
    
Matrix Matrix::operator+(Matrix const &other) const {
    if (d_n != other.d_n or d_m != other.d_m) {throw std::range_error("Matrix addition incompatible!");}
    
    std::vector<double> sum;
    sum.reserve(d_n * d_m);
    for (size_t idx = 0; idx < d_n * d_m; ++idx)
        sum.emplace_back(d_arr[idx] + other.d_arr[idx]);
    
    return Matrix(sum, d_n, d_m);
}

Matrix Matrix::operator*(Matrix const &other) const {
    if (d_m != other.d_n) throw std::range_error("Matrix-Matrix multiplication incompatible!");
    
    Log::debug("Multiplying ") << *this << " with " << other << '\n';

    std::vector<double> res(d_n * other.d_m, 0);

    for (size_t ridx = 0; ridx < d_n; ++ridx) {
        for (size_t cidx = 0; cidx < other.d_m; ++cidx) {
            for (size_t k = 0; k < other.d_n; ++k) {
                res[ridx * d_n + cidx] += d_arr[ridx * d_n + k] * other.d_arr[k * other.d_n + cidx];
            }
        }
    }    
    return Matrix(res, d_n, other.d_m);
}

/// @brief Swap row i with row j. 
void Matrix::swaprows(size_t const i, size_t const j) {
    for (size_t k = 0; k < d_m; ++k)
        std::swap(this->operator()(i, k), this->operator()(j, k));    
}

void Matrix::multiplyrow(size_t const i, double const l) {
    for (size_t k = 0; k < d_m; ++k)
        this->operator()(i, k) *= l;    
}
/// @brief Add row j to row i. 
void Matrix::addrows(size_t const i, size_t const j) {
    for (size_t k = 0; k < d_m; ++k)
        this->operator()(i, k) += this->operator()(j, k);    
}

/// Gaussian eliminate the matrix. 
void Matrix::gaussianelim() {
    for (size_t k = 0; k < d_m; k++)
    {
        // Search pivot row. 
        // Divide by pivot.
    }
    
}
// Matrix operator*(Matrix const &mat, Matrix const &vec) {
//     if (mat.d_m != vec.d_n) throw std::range_error("Matrix-Vector multiplication incompatible!");
    
//     std::vector<double> res(mat.d_n * vec.d_m, 0);

//     for (size_t ridx = 0; ridx < mat.d_n; ++ridx) {
//         for (size_t cidx = 0; cidx < vec.d_m; ++cidx) {
//             for (size_t k = 0; k < vec.d_n; ++k) {
//                 res[ridx * mat.d_n + cidx] += mat.d_arr[ridx * mat.d_n + k] * vec.d_arr[k * vec.d_n + cidx];
//             }
//         }
//     }    
//     return Matrix(res);
// }

Vector solve(Matrix const &A, Vector const &b) {
    // Append the b vector to A. 

    // 
    return Vector::ones(b.d_n);    
}
