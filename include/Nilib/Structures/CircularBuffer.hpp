#ifndef _CIRCULAR_BUFFER_H
#define _CIRCULAR_BUFFER_H

#include <array>
#include <vector>

namespace Nilib {

    // A non-owning class that points to a block of memory.
    struct Buffer
    {
        std::byte *start = nullptr;
        size_t size = 0;

        Buffer() = default;
        Buffer(size_t size) { allocate(size); }

        void allocate(size_t bytes) {
            free();

            start = new std::byte[bytes];
            size = bytes;      
        }

        void free() {
            delete[] start;
            start = nullptr;
            size = 0;
        }

        template<typename Type>
        Type* as() {
            return (Type*)start;
        }


        // Iterate over the buffer. 
        
    };

    // A fixed size Buffer that can 'overflow.' 
    // Old data is overwritten when the buffer overflows.
    class CircularBuffer : public Buffer
    {
        std::byte *writeHead;
        std::byte *readHead;

        

        // Iterators.
    };

    // Normally, if you remove an element from a vector it moves all subsequent elements one place forward.
    // This operation preserves the (sorted) order of elements, but is expensive. 
    // This container instead swaps the last element to the removed element. 
    // No ordering is preserved but removal is in O(1).
    template<typename Type, size_t size>
    class SwapbackVector
    {
        std::vector<Type> d_data;


    };

    // When you add a Type to this container you receive an index associated with that type. 
    // This index points to your original type no matter what. 
    // Removal of types does not invalidate previous types.
    // For example, node_id = PersistentIndex.push(NodeData);
    // PersistentIndex[node_id] == NodeData, unless node_id was removed. 
    template<typename Type, size_t size>
    class PersistentIndex
    {
        std::array<Type, size> d_data;
    };

    // Graph.

}



#endif