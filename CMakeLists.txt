cmake_minimum_required(VERSION 3.10)
project(Nilib VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the external GLFW submodule
if(WIN32)
  add_subdirectory(external/gurobi1103)
endif()

add_subdirectory(external/glfw)
add_subdirectory(external/glad)
# set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build HiGHS as static library" FORCE)
add_subdirectory(external/HiGHS)

# Add subdirectories for the Nilibrary
add_subdirectory(source)

string(COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" NILIB_STANDALONE)

# Only LEVELS higher than or equal to LOG_LEVEL are displayed.
# define LOG_LEVEL_TRACE 1
# define LOG_LEVEL_DEBUG 2
# define LOG_LEVEL_INFO 3
# define LOG_LEVEL_WARNING 4
# define LOG_LEVEL_SUCCESS 5
# define LOG_LEVEL_ERROR 5
# define LOG_LEVEL_PROGRESS 6

target_compile_options(Nilib PRIVATE
  # Supress Microsoft include warnings. 
  -Wno-microsoft-include
  # Release defines
  # $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-DLOG_LEVEL=9>
  $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-DASSERT_OFF>
  # $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-DISABLE_PROFILE>

  # Debug logging level
  $<$<CONFIG:Debug>:-DLOG_LEVEL=1>
  $<$<CONFIG:Debug>:-DTRACK_MEMORY>

  # Debug warnings (GCC / MinGW)
  $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
    -Wimplicit-fallthrough
    -Wunused
    -Woverloaded-virtual
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wuseless-cast
    # -Werror
  >
)

set_property(TARGET Nilib PROPERTY
    INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

set_property(TARGET Nilib PROPERTY
    INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE)


message(STATUS "Nilib Build type: ${CMAKE_BUILD_TYPE}")

if(NILIB_STANDALONE)
  enable_testing()
  add_subdirectory(tests)

  # Build example directory.
  add_subdirectory(examples)
endif()
