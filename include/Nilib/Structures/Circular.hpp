#ifndef _CIRCULAR_BUFFER_H
#define _CIRCULAR_BUFFER_H

#include <array>
#include <vector>
#include "Nilib/Core/Assert.hpp"

namespace Nilib
{
    // A fixed size Buffer that can 'overflow.'
    // Old data is overwritten when the buffer overflows.
    template <typename Element, size_t N>
    class CircularStack
    {
        std::array<Element, N> data;

    public:
        size_t head = 0;
        CircularStack() = default;

        CircularStack(Element const &element)
        {
            data.fill(element);
        }

        // Add a value to the stack.
        void push_back(Element const &value)
        {
            head ++;
            head %= N;
            data[head] = value;
        }
        // Remove a value to the stack.
        // On the next push_back this value is gonna be erased!
        Element &pop_back()
        {
            head = (head + N - 1) % N;
            return data[head + 1];
        }
        // Iterators for range-for compatibility and STL algorithms
        auto begin() noexcept { return data.begin(); }
        auto end() noexcept { return data.end(); }

        auto begin() const noexcept { return data.begin(); }
        auto end() const noexcept { return data.end(); }

        auto cbegin() const noexcept { return data.cbegin(); }
        auto cend() const noexcept { return data.cend(); }

        auto rbegin() noexcept { return data.rbegin(); }
        auto rend() noexcept { return data.rend(); }

        auto rbegin() const noexcept { return data.rbegin(); }
        auto rend() const noexcept { return data.rend(); }

        auto crbegin() const noexcept { return data.crbegin(); }
        auto crend() const noexcept { return data.crend(); }

        // Optional: size, operator[], etc.
        constexpr size_t size() const noexcept { return N; }

        Element &operator[](size_t const i) noexcept { return data[i]; }
        Element const &operator[](size_t const i) const noexcept { return data[i]; }

        Element &at(size_t const i) noexcept
        {
            CORE_ASSERT(i < N);
            return data[i];
        }
        Element const &at(size_t const i) const noexcept
        {
            CORE_ASSERT(i < N);
            return data[i];
        }
    };

    // Allows an additional read ptr.
    // Old data is overwritten when the buffer overflows.
    // template <typename Element, size_t N>
    // class CircularQueue : public CircularStack<Element, N>
    // {
    //     size_t readptr = 0;

    // public:
    //     CircularQueue() = default;

    //     CircularQueue(Element const &element)
    //         : CircularStack<Element, N>(element)
    //     {
    //     }
    // };

    // Normally, if you remove an element from a vector it moves all subsequent elements one place forward.
    // This operation preserves the (sorted) order of elements, but is expensive.
    // This container instead swaps the last element to the removed element.
    // No ordering is preserved but removal is in O(1).
    template <typename Type, size_t size>
    class SwapbackVector
    {
        std::vector<Type> d_data;
    };



    // Graph.
}

#endif