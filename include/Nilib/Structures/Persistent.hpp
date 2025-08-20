#ifndef _PERSISTENT_INDEX_VEC_HPP
#define _PERSISTENT_INDEX_VEC_HPP

#include "Nilib/Core/Assert.hpp"

#include <vector>
#include <numeric>
#include <algorithm>

namespace Nilib
{

    // When you add a Type to this container you receive an index associated with that type.
    // This index points to your original type no matter what.
    // Removal of types does not invalidate previous types.
    // For example, node_id = PersistentIndex.push(NodeData);
    // PersistentIndex[node_id] == NodeData, unless node_id was removed.
    template <typename Type, typename index_type>
    class PersistentIndexVector
    {
    public:
        using index = index_type;

        PersistentIndexVector() = default;

        PersistentIndexVector(index const reserve)
        {
            d_data.resize(reserve);
            d_free.resize(reserve);
            std::iota(d_free.begin(), d_free.end(), 0);
        }

        // Add a type and return its persistent index
        index add(Type const &type)
        {
            if (d_free.empty())
            {
                index const id = d_data.size();
                d_data.push_back(type);
                return id;
            }
            else
            {
                // Insert in first free entry.
                index const idx = d_free.back();
                d_data[idx] = std::move(type);
                d_free.pop_back();
                return idx;
            }
        }

        // Remove a type by value
        void remove(Type const &type)
        {
            remove(static_cast<index>(std::distance(d_data.begin(), std::find(d_data.begin(), d_data.end(), type))));
        }

        // Remove a type by index
        void remove(index const idx)
        {
            ASSERT(std::find(d_free.begin(), d_free.end(), idx) == d_free.end(), "Element at idx", idx, "has already been deleted!");
            ASSERT(idx < d_data.size(), "Removing element", idx, "in PersistentIndexVector with size", d_data.size());

            d_free.push_back(idx);
        }

        // Get the number of elements stored.
        size_t size() const
        {
            return d_data.size() - d_free.size();
        }

        // Get the number of elements (including inactive) stored.
        size_t capacity() const
        {
            return d_data.size();
        }

        Type &operator[](index const idx)
        {
            ASSERT(std::find(d_free.begin(), d_free.end(), idx) == d_free.end(), "Element at idx", idx, "has already been deleted!");
            ASSERT(idx < d_data.size(), "Accessing element", idx, "in PersistentIndexVector with size", d_data.size());
            return d_data[idx];
        }

        Type const &operator[](index const idx) const
        {
            ASSERT(std::find(d_free.begin(), d_free.end(), idx) == d_free.end(), "Element at idx", idx, "has already been deleted!");
            ASSERT(idx < d_data.size(), "Accessing element", idx, "in PersistentIndexVector with size", d_data.size());
            return d_data[idx];
        }

        bool contains(index const idx) const
        {
            return !empty() && std::find(d_free.begin(), d_free.end(), idx) == d_free.end();
        }

        bool empty() const
        {
            return size() == 0;
        }

        // Compacts the class but Invalidates indices!!
        void shrink_to_fit()
        {
            // Mark all free indices for fast lookup
            std::vector<bool> is_free(d_data.size(), false);
            for (index idx : d_free)
                is_free[idx] = true;

            // Compact the data in-place
            index write = 0;
            for (index read = 0; read < d_data.size(); ++read)
            {
                if (is_free[read])
                    continue;

                if (write != read)
                    d_data[write] = std::move(d_data[read]);

                ++write;
            }

            // Resize to remove the trailing "freed" elements
            d_data.resize(write);
            d_free.clear();
        }

        void clear()
        {
            d_data.clear();
            d_free.clear();
        }

        class iterator
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Type;
            using difference_type = std::ptrdiff_t;
            using pointer = Type *;
            using reference = Type &;

            iterator(PersistentIndexVector<Type, index_type> *parent, index current)
                : d_parent(parent), d_current(current)
            {
                skip_invalid();
            }

            reference operator*() const { return d_parent->d_data[d_current]; }
            pointer operator->() const { return &d_parent->d_data[d_current]; }

            iterator &operator++()
            {
                ++d_current;
                skip_invalid();
                return *this;
            }

            iterator operator++(int)
            {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            bool operator==(const iterator &other) const
            {
                return d_current == other.d_current;
            }

            bool operator!=(const iterator &other) const
            {
                return !(*this == other);
            }

            index getIndex() const { return d_current; }

        private:
            PersistentIndexVector<Type, index_type> *d_parent;
            index d_current;

            void skip_invalid()
            {
                while (d_current < d_parent->d_data.size() &&
                       std::find(d_parent->d_free.begin(), d_parent->d_free.end(), d_current) != d_parent->d_free.end())
                {
                    ++d_current;
                }
            }
        };

        class const_iterator
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Type const;
            using difference_type = std::ptrdiff_t;
            using pointer = Type const *;
            using reference = Type const &;

            const_iterator(PersistentIndexVector<Type, index_type> const *parent, index current)
                : d_parent(parent), d_current(current)
            {
                skip_invalid();
            }

            reference operator*() const { return d_parent->d_data[d_current]; }
            pointer operator->() const { return &d_parent->d_data[d_current]; }

            const_iterator &operator++()
            {
                ++d_current;
                skip_invalid();
                return *this;
            }

            const_iterator operator++(int)
            {
                const_iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            bool operator==(const_iterator const &other) const
            {
                return d_current == other.d_current;
            }

            bool operator!=(const_iterator const &other) const
            {
                return !(*this == other);
            }

            index getIndex() const { return d_current; }

        private:
            PersistentIndexVector<Type, index_type> const *d_parent;
            index d_current;

            void skip_invalid()
            {
                while (d_current < d_parent->d_data.size() &&
                       std::find(d_parent->d_free.begin(), d_parent->d_free.end(), d_current) != d_parent->d_free.end())
                {
                    ++d_current;
                }
            }
        };

        iterator begin() { return iterator(this, 0); }
        iterator end() { return iterator(this, d_data.size()); }

        const_iterator begin() const { return const_iterator(this, 0); }
        const_iterator end() const { return const_iterator(this, d_data.size()); }

        const_iterator cbegin() const { return const_iterator(this, 0); }
        const_iterator cend() const { return const_iterator(this, d_data.size()); }

    private:
        std::vector<Type> d_data;
        std::vector<index> d_free; // Free indices.
    };

} // namespace Nilib

#endif