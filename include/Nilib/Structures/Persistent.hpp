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
    // Elements are saved contiguously. Thus looping over the PIndexVectoris linear
    // Retrieval by node_id is O(1).
    // Deleting is O(1): swap back with the last data point.
    // Addition is O(1): add to the end.
    template <typename Type>
    class PIndexVector
    {
    public:
        using index = uint16_t;

        PIndexVector() = default;
        PIndexVector(PIndexVector const &) = default;
        void reserve(index size);

        // Add or remove.
        index add(Type const &type);
        void remove(index);
        void clear()
        {
            d_data.clear();
            d_head.clear();
            d_tail.clear();
        }

        // Accessors
        Type &operator[](index id)
        {
            ASSERT(d_head[id] < d_data.size(), "Index does not exist anymore!");
            return d_data[d_head[id]];
        };
        Type const operator[](index id) const
        {
            ASSERT(d_head[id] < d_data.size(), "Index does not exist anymore!");
            return d_data[d_head[id]];
        };

        // check if an index exists.
        bool contains(index id) const { return id < d_head.size() and d_head[id] < d_data.size(); }
        size_t size() const { return d_data.size(); }
        bool empty() const { return d_data.empty(); }

        class Iterator
        {
        public:
            Iterator(index pos, PIndexVector *parent) : d_pos(pos), d_parent(parent) {}
            bool operator==(Iterator other) const { return other.d_pos == d_pos and other.d_parent == d_parent; }
            void operator++() { d_pos++; }
            Type &operator*() { return d_parent->d_data[d_pos]; }

        private:
            index d_pos;
            PIndexVector *d_parent;
        };

        // Allows loops such as [indx, value]
        class IndexIterator
        {
        public:
            IndexIterator(index pos, PIndexVector *parent) : d_pos(pos), d_parent(parent) {}
            bool operator==(IndexIterator other) const { return other.d_pos == d_pos and other.d_parent == d_parent; }
            void operator++() { d_pos++; }
            std::pair<index, Type> operator*() { return std::make_pair(d_parent->d_tail[d_pos], d_parent->d_data[d_pos]); }

        private:
            index d_pos;
            PIndexVector *d_parent;
        };

        // Support for looping over the PIndexVector
        PIndexVector<Type>::Iterator begin() { return Iterator(0, this); };
        PIndexVector<Type>::Iterator end() { return Iterator(d_data.size(), this); };
        PIndexVector<Type>::IndexIterator ibegin() { return IndexIterator(0, this); };
        PIndexVector<Type>::IndexIterator iend() { return IndexIterator(d_data.size(), this); };

    private:
        std::vector<Type> d_data;  // Stores the data.
        std::vector<index> d_head; // Stores the index -> data position.
        std::vector<index> d_tail; // Stores the data position -> index.
    };

    template <typename Type>
    void PIndexVector<Type>::reserve(PIndexVector<Type>::index size)
    {
        d_data.reserve(size);
        d_head.reserve(size);
        d_tail.reserve(size);
    }

    template <typename Type>
    PIndexVector<Type>::index PIndexVector<Type>::add(Type const &data)
    {
        if (d_head.size() == d_data.size())
        {
            d_head.push_back(d_data.size());
            d_tail.push_back(d_data.size());
        }
        d_data.push_back(data);
        return d_tail[d_tail.size() - 1];
    }

    template <typename Type>
    void PIndexVector<Type>::remove(PIndexVector<Type>::index index)
    {
        ASSERT(contains(index), "Attempt to remove index", index, "which has not been reassigned!");
        // Swap back the data with the last entry.
        std::swap(d_data[index], d_data[d_data.size() - 1]);
        // Swap the data pos -> index vector!
        std::swap(d_tail[index], d_tail[d_tail.size() - 1]);

        // Invariants.
        d_head[d_tail[index]] = index;
        d_head[d_tail[d_tail.size() - 1]] = d_tail.size() - 1;

        // Remove the last entry.
        d_data.erase(--d_data.end());
    }
} // namespace Nilib

#endif