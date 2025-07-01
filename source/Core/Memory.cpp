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
    LOG_DEBUG("Total Allocated:", formatBytes(allocations.sum()));
    LOG_DEBUG("Total Freed:    ", formatBytes(deallocations.sum()));
    LOG_DEBUG("Leaked:         ", formatBytes(allocations.sum() - deallocations.sum()));
    LOG_DEBUG("=======================");
}

MemoryTracker::~MemoryTracker()
{
    report();
}

std::string MemoryTracker::formatBytes(uint64_t bytes, bool useBinary) const
{
    const char *siUnits[] = {"B", "KB", "MB", "GB", "TB", "PB"};
    const char *iecUnits[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB"};

    double size = static_cast<double>(bytes);
    int unit = 0;

    int divisor = useBinary ? 1024 : 1000;
    const char **units = useBinary ? iecUnits : siUnits;

    while (size >= divisor && unit < 5)
    {
        size /= divisor;
        ++unit;
    }

    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << size << " " << units[unit];
    return out.str();
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


#ifdef TRACK_MEMORY

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

#endif