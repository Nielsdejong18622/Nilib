#ifndef _MEMORY_H
#define _MEMORY_H

#include "Nilib/Math/Stats.hpp"
#include "Nilib/Logger/Log.hpp"

#include <cstdlib> // for malloc and free
#include <unordered_map>
#include <mutex>

#include <new>

namespace Nilib
{
    // Global memory tracker
    class MemoryTracker
    {
        std::string formatBytes(uint64_t bytes, bool useBinary = true) const;

    public:
        Nilib::RunningStats allocations;
        Nilib::RunningStats deallocations;

        ~MemoryTracker();
        void add(size_t size) noexcept;

        void remove(size_t ptr) noexcept;

        void report() const noexcept;
    };
} // namespace Nilib


// Align memory (optional but safe)
void *allocateWithSize(std::size_t size);
void deallocateWithSize(void *userPtr) noexcept;
#ifdef TRACK_MEMORY

// Single object
void *operator new(std::size_t size);
void operator delete(void *ptr) noexcept;
void operator delete(void *ptr, std::size_t size) noexcept;

// Arrays
void *operator new[](std::size_t size);
void operator delete[](void *ptr) noexcept;
void operator delete[](void *ptr, std::size_t size) noexcept;
#endif

#endif