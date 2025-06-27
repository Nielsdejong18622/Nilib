#include "Nilib/Core/Memory.hpp"

using namespace Nilib;

void MemoryTracker::add(size_t size) noexcept
{
    ;
    allocations.push(size);
}

void MemoryTracker::remove(size_t size) noexcept
{
    deallocations.push(size);
}

void MemoryTracker::report() const noexcept
{
    LOG_DEBUG("==== Memory Tracker Report ====");
    LOG_DEBUG() << "Total Allocated: " << allocations.sum() << " bytes\n";
    LOG_DEBUG() << "Total Freed:     " << deallocations.sum() << " bytes\n";
    LOG_DEBUG() << "Leaked:          " << (allocations.sum() - deallocations.sum()) << " bytes\n";
    LOG_DEBUG("=======================");
}

MemoryTracker::~MemoryTracker()
{
    report();
}

// Declare global static instance
static MemoryTracker gMemoryTracker;

// Align size for pointer arithmetic (optional, but good practice)
constexpr size_t HEADER_SIZE = sizeof(std::size_t);

// Safe allocation
void *allocateWithHeader(std::size_t size)
{
    void *raw = std::malloc(size);
    if (!raw)
        throw std::bad_alloc();

    gMemoryTracker.add(size);
    return raw;
}

// Safe deallocation
void deallocateWithHeader(void *userPtr, size_t size) noexcept
{
    if (!userPtr)
        return;
    std::free(userPtr);
    gMemoryTracker.remove(size);
}

// Single object
void *operator new(std::size_t size)
{
    return allocateWithHeader(size);
}
void operator delete(void *ptr) noexcept
{
    deallocateWithHeader(ptr, 0);
}
void operator delete(void *ptr, std::size_t size) noexcept
{
    deallocateWithHeader(ptr, size);
}

// Arrays
void *operator new[](std::size_t size)
{
    return allocateWithHeader(size);
}
void operator delete[](void *ptr) noexcept
{
    deallocateWithHeader(ptr, 0);
}
void operator delete[](void *ptr, std::size_t size) noexcept
{
    deallocateWithHeader(ptr, size);
}