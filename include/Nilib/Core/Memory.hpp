#ifndef _MEMORY_HPP
#define _MEMORY_HPP

#include <cstddef>
#include <string>

namespace Nilib
{
  std::string format_bytes(uint64_t bytes, bool useBinary = true);
}

#endif