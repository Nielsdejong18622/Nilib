#ifndef _STACK_VECTOR_HPP
#define _STACK_VECTOR_HPP

#include <array>
#include <vector>

namespace Nilib
{
    // Forward declaration.
    template <typename T, size_t N>
    class StackVector;

    template <typename T, size_t N>
    class StackVectorIterator
    {
        using ArrayPtr = T *;
        using VectorIter = typename std::vector<T>::iterator;

        ArrayPtr arr_ptr = nullptr;
        VectorIter vec_iter;
        size_t index = 0;
        size_t head = 0;
        StackVector<T, N> *parent = nullptr;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        StackVectorIterator(StackVector<T, N> *parent, size_t pos)
            : parent(parent), index(pos), head(parent->head)
        {
            arr_ptr = parent->data.data();
            vec_iter = parent->a_data.begin();
        }

        reference operator*() const
        {
            if (index < N)
                return arr_ptr[index];
            else
                return *(vec_iter + (index - N));
        }

        StackVectorIterator &operator++()
        {
            ++index;
            return *this;
        }

        StackVectorIterator operator++(int)
        {
            StackVectorIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const StackVectorIterator &other) const
        {
            return index == other.index;
        }

        bool operator!=(const StackVectorIterator &other) const
        {
            return !(*this == other);
        }
    };

    // Saves first N elements on the stack, additional items are saved on the heap.
    template <typename Element, size_t N>
    class StackVector
    {
        std::array<Element, N> data;
        std::vector<Element> a_data; // Holds pointers when empty size(vector{}) = 24bytes.
        size_t head = 0;

    public:
        StackVector() = default;

        Element &operator[](size_t const idx) { return (idx < N) ? data[idx] : a_data[idx - N]; }
        Element const &operator[](size_t const idx) const { return (idx < N) ? data[idx] : a_data[idx - N]; }

        Element const &at(size_t const idx) const
        {
            CORE_ASSERT(idx < N + a_data.size());
            return operator[](idx);
        };
        Element &at(size_t const idx)
        {
            CORE_ASSERT(idx < N + a_data.size());
            return operator[](idx);
        };

        size_t const size() const { return head; }

        void clear()
        {
            a_data.clear();
            head = 0;
        }

        Element &pop_back()
        {
            head--;
            return operator[](head + 1);
        }
        Element &back()
        {
            return operator[](head);
        }

        void push_back(Element const &element)
        {
            if (head < N)
            {
                data[head] = element;
            }
            else
            {
                a_data.push_back(element);
            }
            head++;
        }

        using iterator = StackVectorIterator<Element, N>;
        using const_iterator = StackVectorIterator<const Element, N>;

        iterator begin() { return iterator(this, 0); }
        iterator end() { return iterator(this, head); }

        const_iterator begin() const { return const_iterator(this, 0); }
        const_iterator end() const { return const_iterator(this, head); }

        const_iterator cbegin() const { return const_iterator(this, 0); }
        const_iterator cend() const { return const_iterator(this, head); }

        friend class StackVectorIterator<Element, N>;
    };
} // namespace Nilib

#endif