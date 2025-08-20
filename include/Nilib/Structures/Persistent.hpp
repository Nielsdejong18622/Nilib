#ifndef _PERSISTENT_INDEX_VEC_HPP
#define _PERSISTENT_INDEX_VEC_HPP

#include <vector>
#include <deque>

namespace Nilib
{

    // When you add a Type to this container you receive an index associated with that type.
    // This index points to your original type no matter what.
    // Removal of types does not invalidate previous types.
    // For example, node_id = PersistentIndex.push(NodeData);
    // PersistentIndex[node_id] == NodeData, unless node_id was removed.
    template <typename Type>
    class PersistentIndexVector
    {
    public:
        using index = size_t;

        // Add a type and return its persistent index
        index add(Type const &type)
        {
            if (d_free.empty())
            {
                d_data.emplace_back(type);
                return d_data.size();
            }
            else
            {
                // Insert in first free entry.
                index const idx = d_free.front();
                d_data[idx] = std::move(type);
                d_free.pop_front();
                return idx
            }
        }

        // Remove a type by value
        void remove(Type const &type)
        {
            remove(std::find(d_data.begin(), d_data.end(), type));
        }

        // Remove a type by index
        void remove(index const idx)
        {
            ASSERT(std::find(d_free.begin(), d_free.end(), idx) == d_free.end(), "Element at idx", idx, "has already been deleted!");
            ASSERT(idx < d_data.size(), "Removing element", idx, "in PersistentIndexVector with size", d_data.size());
        }

        // Get the number of elements stored.
        size_t size() const
        {
            return d_data.size() - d_free.size();
        }

        Type &operator[](index const idx)
        {
            ASSERT(std::find(d_free.begin(), d_free.end(), idx) == d_free.end(), "Element at idx", idx, "has already been deleted!");
            ASSERT(idx < d_data.size(), "Accessing element", idx, "in PersistentIndexVector with size", d_data.size());
            return d_data[idx];
        }

    private:
        std::vector<Type> d_data;
        std::deque<int> d_free; // Free indices.
    };

} // namespace Nilib

#endif