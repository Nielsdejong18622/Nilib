#include "Nilib/Math/RNG.hpp"

namespace Nilib
{
    template <size_t stream>
    uint64_t Nilib::RAN<stream>::state = 00123;

    // Optional: specialize for stream 0, 1, 2
    template <>
    uint64_t Nilib::RAN<0>::state = 127;
    template <>
    uint64_t Nilib::RAN<1>::state = 0xdead;
    template <>
    uint64_t Nilib::RAN<3>::state = 9042582;
    template <>
    uint64_t Nilib::RAN<4>::state = 83672389;

} // namespace Nilib
