#ifndef _MATRIX_STORAGE_HPP
#define _MATRIX_STORAGE_HPP

#include <valarray>
#include <array>

#include <numeric>
#include <functional>
#include "Nilib/Core/Assert.hpp"

namespace Nilib
{

    // Stores dense matrix data on the stack.
    template <size_t n, size_t m, typename type = float>
    struct StaticMatrixData
    {
        std::array<type, n * m> d_data;

        StaticMatrixData() = default;

        StaticMatrixData(size_t const rows, size_t const cols)
        {
            CORE_ASSERT(rows == n);
            CORE_ASSERT(cols == m);
            d_data.fill(0);
        }

        StaticMatrixData(std::initializer_list<type> const &list)
        {
            CORE_ASSERT(list.size() == size())
            std::move(list.begin(), list.end(), d_data.begin());
        }

        StaticMatrixData(std::initializer_list<std::initializer_list<type>> const &list)
        {
            CORE_ASSERT(list.size() == n);
            CORE_ASSERT(list.begin()->size() == m);
            size_t idx = 0;
            for (auto const &row : list)
                for (auto const &val : row)
                    d_data[idx++] = val;
        }

        type &operator()(size_t const idx)
        {
            CORE_ASSERT(idx < d_data.size());
            return d_data[idx];
        };

        type operator()(size_t const idx) const
        {
            CORE_ASSERT(idx < d_data.size());
            return d_data[idx];
        };

        type operator()(size_t const row, size_t col) const { return operator()(row * cols() + col); }
        type &operator()(size_t const row, size_t const col) { return operator()(row * cols() + col); }

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

    // Stores sparse matrix data (on the heap)
    template <typename type = float>
    struct SparseMatrixData
    {
        size_t d_rows = 0;
        size_t d_cols = 0;
        mutable std::unordered_map<size_t, type> d_data; // Holds only the non-zero entries.

        SparseMatrixData() = default;

        SparseMatrixData(std::initializer_list<type> const &list)
            : d_rows(1), d_cols(list.size())
        {
            for (size_t idx = 0; idx < list.size(); ++idx)
            {
                if (list[idx] != static_cast<type>(0))
                {
                    d_data[idx] = list[idx];
                }
            }
        }

        SparseMatrixData(size_t const rows, size_t const cols)
            : d_rows(rows), d_cols(cols) {}

        SparseMatrixData(size_t const rows, size_t const cols, std::unordered_map<size_t, bool> const &data)
            : d_rows(rows), d_cols(cols), d_data(data) {}

        SparseMatrixData(std::initializer_list<std::initializer_list<type>> const &list)
            : d_rows(list.size()), d_cols(list.begin()->size())
        {
            size_t idx = 0;
            for (const auto &row : list)
                for (const auto &val : row)
                {
                    if (val != static_cast<type>(0))
                        d_data[idx] = val;
                    idx++;
                }
        }

        inline type &operator()(size_t const idx)
        {
            if (d_data.contains(idx))
            {
                return d_data.at(idx);
            }
            d_data[idx] = 0;
            return d_data[idx];
        };

        inline type operator()(size_t const idx) const
        {
            if (d_data.contains(idx))
            {
                return d_data.at(idx);
            }
            return 0;
        };

        inline type operator()(size_t const row, size_t col) const { return operator()(row * cols() + col); }
        inline type &operator()(size_t const row, size_t const col) { return operator()(row * cols() + col); }

        inline size_t rows() const noexcept { return d_rows; }
        inline size_t cols() const noexcept { return d_cols; }
        inline size_t size() const noexcept { return d_rows * d_cols; }

        // type sum() const {return std::accumulate(std::begin(d_data), std::end(d_data), 0.0); }
        type sum() const
        {
            type sum = 0;
            for (auto &&[k, v] : d_data)
            {
                sum += v;
            }
            return sum;
        }

        template <typename Callable>
        void apply(Callable &&fun)
        {
            std::transform(std::begin(d_data), std::end(d_data), std::begin(d_data), std::forward<Callable>(fun));
        }
    };

    // Stores dense matrix data on the heap.
    template <typename type = float>
    struct DynamicMatrixData
    {
        size_t d_rows = 0;
        size_t d_cols = 0;          // Can be infered from d_data.size() / d_rows.
        std::valarray<type> d_data; // Dynamically allocated on the heap. Internally stores three pointers.

        DynamicMatrixData() = default;

        DynamicMatrixData(std::initializer_list<type> const &list)
            : d_rows(1), d_cols(list.size()), d_data(list) {}

        DynamicMatrixData(size_t const rows, size_t const cols)
            : d_rows(rows), d_cols(cols), d_data(0.0, rows * cols) {}

        DynamicMatrixData(size_t const rows, size_t const cols, std::valarray<type> const &data)
            : d_rows(rows), d_cols(cols), d_data(data) {}

        DynamicMatrixData(std::initializer_list<std::initializer_list<type>> const &list)
            : d_rows(list.size()), d_cols(list.begin()->size())
        {
            d_data.resize(d_rows * list.begin()->size(), 0);
            size_t idx = 0;
            for (const auto &row : list)
                for (const auto &val : row)
                    d_data[idx++] = val;
        }

        inline type &operator()(size_t const idx)
        {
            ASSERT(idx < d_data.size(), idx, '<', d_data.size());
            return d_data[idx];
        };

        inline type operator()(size_t const idx) const
        {
            ASSERT(idx < d_data.size(), idx, '<', d_data.size());
            return d_data[idx];
        };

        inline type operator()(size_t const row, size_t col) const { return operator()(row * cols() + col); }
        inline type &operator()(size_t const row, size_t const col) { return operator()(row * cols() + col); }

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