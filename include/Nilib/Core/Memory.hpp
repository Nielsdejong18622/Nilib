#ifndef _MEMORY_H
#define _MEMORY_H

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Math/Stats.hpp"

#include <cstdlib> // for malloc and free
#include <mutex>
#include <unordered_map>

#include <new>

namespace Nilib
{
  // For formatting a count of bytes into human readable format.
  std::string format_bytes(uint64_t bytes, bool useBinary = true);

  // Global memory tracker
  class MemoryTracker
  {

  public:
    Nilib::RunningStats<size_t> allocations;
    Nilib::RunningStats<size_t> deallocations;

    ~MemoryTracker();
    void add(size_t size) noexcept;

    void remove(size_t ptr) noexcept;

    void report() const noexcept;
  };
} // namespace Nilib

// Align memory (optional but safe)
void *allocateWithSize(std::size_t size);
void deallocateWithSize(void *userPtr) noexcept;

#ifndef TRACK_MEMORY
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