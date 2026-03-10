#include "Nilib/Core/Memory.hpp"
#include "Nilib/Logger/Log.hpp"
#include "Memory.hpp"

std::string Nilib::format_bytes(size_t bytes, bool useBinary)
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
