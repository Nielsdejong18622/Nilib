#include "Nilib/Core/Memory.hpp"

using namespace Nilib;

std::string Nilib::format_bytes(uint64_t bytes, bool useBinary)
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

void MemoryTracker::add(size_t size) noexcept
{
    allocations.push(size);
}

void MemoryTracker::remove(size_t size) noexcept
{
    deallocations.push(size);
}

void MemoryTracker::report() const noexcept
{
    LOG_DEBUG("==== Memory Tracker Report ====");
    LOG_DEBUG("Total Allocated:", allocations.n(), format_bytes(allocations.sum()));
    LOG_DEBUG("Total Freed:    ", deallocations.n(), format_bytes(deallocations.sum()));
    LOG_DEBUG("Leaked:         ", format_bytes(allocations.sum() - deallocations.sum()), "in",
              allocations.n() - deallocations.n(), "chunks");
    LOG_DEBUG("=======================");
}

MemoryTracker::~MemoryTracker()
{
    report();
}

// Declare global static instance
static MemoryTracker gMemoryTracker;

#ifdef TRACK_MEMORY
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
#endif