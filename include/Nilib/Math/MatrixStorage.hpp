#ifndef _MATRIX_STORAGE_HPP
#define _MATRIX_STORAGE_HPP

#include <valarray>
#include <array>

#include <numeric>
#include <functional>
#include "Nilib/Core/Assert.hpp"

namespace Nilib{

    // Abstract internal storage class. 
    template <typename type = float>
    struct MatrixData
    {
        virtual ~MatrixData() = default;

        virtual type operator()(size_t const row, size_t col) const { return operator()(row * cols() + col); }
        virtual type &operator()(size_t const row, size_t const col) { return operator()(row * cols() + col); }

        virtual type operator()(size_t const idx) const = 0;
        virtual type &operator()(size_t const idx) = 0;

        virtual size_t cols() const = 0;
        virtual size_t rows() const = 0;
        size_t size() const { return rows() * cols(); }


        virtual type sum() const = 0;
        virtual void apply(std::function<type(type)> fun) = 0;
    };

    // Stores dense matrix data on the stack. 
    template <size_t n, size_t m, typename type = float>
    struct StaticMatrixData : public MatrixData<type>
    {
        std::array<type, n * m> d_data;
        
        StaticMatrixData() = default;
        StaticMatrixData(size_t const rows, size_t const cols) {
            CORE_ASSERT(rows == n);
            CORE_ASSERT(cols == m);
            d_data.fill(0);
        }
        StaticMatrixData(std::initializer_list<type> const &list)
        {
            std::copy(list.begin(), list.end(), d_data.begin());
        }

        
        type &operator()(size_t const idx) {
            return d_data[idx];
        };
        
        type operator()(size_t const idx) const {
            return d_data[idx];
        };
        
        type operator()(size_t const row, size_t col) const { return operator()(row * cols() + col); }
        type &operator()(size_t const row, size_t const col) { return operator()(row * cols() + col); }

        size_t rows() const { return n; }
        size_t cols() const { return m; }

        type sum() const {return std::accumulate(d_data.begin(), d_data.end(), 0.0); }
        void apply(std::function<type(type)> fun) { std::transform(d_data.begin(), d_data.end(), d_data.begin(), fun); }

    };

    // Stores dense matrix data on the heap. 
    template <typename type = float>
    struct DynamicMatrixData : public MatrixData<type>
    {
        size_t d_rows = 0;
        //size_t d_cols = 0; // Can be infered from d_data.size() / d_rows.
        std::valarray<type> d_data;
        
        DynamicMatrixData() = default;
        
        DynamicMatrixData(std::initializer_list<type> const &list)
        : d_data(list), d_rows(1) {}

        DynamicMatrixData(size_t const rows, size_t const cols) 
        : d_rows(rows), d_data(rows * cols) {}

        DynamicMatrixData(std::initializer_list<std::initializer_list<type>> const &list)
        : d_rows(list.size()) 
        {
            d_data.resize(d_rows * list.begin()->size(), 0);
            size_t idx = 0;
            for (const auto& row : list) 
                for (const auto& val : row)
                    d_data[idx++] = val;
        }
        
        type &operator()(size_t const idx) {
            return d_data[idx];
        };
        
        type operator()(size_t const idx) const {
            return d_data[idx];
        };

        type operator()(size_t const row, size_t col) const { return operator()(row * cols() + col); }
        type &operator()(size_t const row, size_t const col) { return operator()(row * cols() + col); }

        size_t rows() const { return d_rows; }
        size_t cols() const { return (d_rows > 0) ? (d_data.size() / d_rows) : 0; }

        type sum() const {return (d_rows > 0) ? d_data.sum() : 0; }
        void apply(std::function<type(type)> fun) { std::transform(std::begin(d_data), std::end(d_data), std::begin(d_data), fun); }
    };
}
#endif