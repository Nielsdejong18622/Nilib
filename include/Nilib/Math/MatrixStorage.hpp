#ifndef _MATRIX_STORAGE_HPP
#define _MATRIX_STORAGE_HPP

#include <valarray>
#include <array>

#include <numeric>
#include <functional>
#include <algorithm>
#include "Nilib/Core/Assert.hpp"

namespace Nilib
{

    // Stores dense matrix data on the stack.
    template <size_t n, size_t m, typename type = float>
    struct StaticMatrixData
    {
        std::array<type, n * m> d_data;

        static constexpr size_t nrow = n;
        static constexpr size_t ncol = m;

        StaticMatrixData(size_t rows = n, size_t cols = m, type val = 0)
        {
            CORE_ASSERT(rows == n);
            CORE_ASSERT(cols == m);
            d_data.fill(val);
        }

        StaticMatrixData(std::initializer_list<type> const &list)
        {
            CORE_ASSERT(list.size() == size())
            std::move(list.begin(), list.end(), d_data.begin());
        }

        StaticMatrixData(std::initializer_list<std::initializer_list<type>> const &list)
        {
            CORE_ASSERT(list.size() == n);
            size_t idx = 0;
            for (auto const &row : list)
            {
                CORE_ASSERT(row->size() == m);
                for (auto const &val : row)
                    d_data[idx++] = val;
            }
        }

        type &operator()(size_t idx)
        {
            CORE_ASSERT(idx < d_data.size());
            return d_data[idx];
        };

        type operator()(size_t idx) const
        {
            CORE_ASSERT(idx < d_data.size());
            return d_data[idx];
        };

        bool operator==(StaticMatrixData const &other) const
        {
            return d_data == other.d_data;
        }

        type operator()(size_t row, size_t col) const { return operator()(row * cols() + col); }
        type &operator()(size_t row, size_t col) { return operator()(row * cols() + col); }

        constexpr size_t rows() const { return n; }
        constexpr size_t cols() const { return m; }
        constexpr size_t size() const { return n * m; }

        type sum() const { return std::accumulate(d_data.begin(), d_data.end(), 0.0); }

        template <typename Callable>
        void apply(Callable &&fun)
        {
            std::transform(d_data.begin(), d_data.end(), d_data.begin(), std::forward<Callable>(fun));
        }
    };

    // Stores dense matrix data on the heap.
    template <typename type = float>
    struct DynamicMatrixData
    {
        size_t d_rows = 0;
        size_t d_cols = 0;             // Can be infered from d_data.size() / d_rows.
        std::valarray<type> d_data{0}; // Dynamically allocated on the heap. Internally stores three pointers.

        DynamicMatrixData() = default;

        DynamicMatrixData(size_t rows, size_t cols, type val = 0.0)
            : d_rows(rows), d_cols(cols), d_data(val, rows * cols) {}

        DynamicMatrixData(std::initializer_list<type> const &list)
            : d_rows(1), d_cols(list.size()), d_data(list) {}

        DynamicMatrixData(std::initializer_list<std::initializer_list<type>> const &list)
            : d_rows(list.size()), d_cols(list.begin()->size())
        {
            d_data.resize(d_rows * list.begin()->size(), 0);
            size_t idx = 0;
            for (const auto &row : list)
                for (const auto &val : row)
                    d_data[idx++] = val;
        }

        bool operator==(DynamicMatrixData const &other) const
        {
            return d_rows == other.d_rows and
                   d_cols == other.d_cols and
                   d_data == other.d_data;
        }

        inline type &operator()(size_t idx)
        {
            ASSERT(idx < d_data.size(), idx, '<', d_data.size());
            return d_data[idx];
        };

        inline type operator()(size_t idx) const
        {
            ASSERT(idx < d_data.size(), idx, '<', d_data.size());
            return d_data[idx];
        };

        inline type operator()(size_t row, size_t col) const { return operator()(row * cols() + col); }
        inline type &operator()(size_t row, size_t col) { return operator()(row * cols() + col); }

        inline size_t rows() const noexcept { return d_rows; }
        inline size_t cols() const noexcept { return d_cols; }
        inline size_t size() const noexcept { return d_data.size(); }

        // type sum() const {return std::accumulate(std::begin(d_data), std::end(d_data), 0.0); }
        type sum() const { return d_data.sum(); }

        template <typename Callable>
        void apply(Callable &&fun)
        {
            std::transform(std::begin(d_data), std::end(d_data), std::begin(d_data), std::forward<Callable>(fun));
        }
    };
}
#endif