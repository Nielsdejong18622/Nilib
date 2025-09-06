cmake_minimum_required(VERSION 3.10)
project(Nilib VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the external GLFW submodule
if(WIN32)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/gurobi1103)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glad)

# Add subdirectories for the Nilibrary
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source)

string(COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" NILIB_STANDALONE)

# Only LEVELS higher than or equal to LOG_LEVEL are displayed.
# define LOG_LEVEL_TRACE 1
# define LOG_LEVEL_DEBUG 2
# define LOG_LEVEL_INFO 3
# define LOG_LEVEL_WARNING 4
# define LOG_LEVEL_SUCCESS 5
# define LOG_LEVEL_ERROR 5
# define LOG_LEVEL_PROGRESS 6

# Definitons of Preprocessor directives.
target_compile_options(Nilib PRIVATE
  ${CMAKE_CXX_FLAGS}
  $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-DASSERT_OFF>
  $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-DENABLE_PROFILE=0>
  # $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-LOG_LEVEL=5> # Display only progress and errors.
  $<$<CONFIG:Debug>:-LOG_LEVEL=1>
  $<$<CONFIG:Debug>:-Wall>
  $<$<CONFIG:Debug>:-Werror>
  $<$<CONFIG:Debug>:-pedantic>
)
message(STATUS "Nilib Build type: ${CMAKE_BUILD_TYPE}")

if(NILIB_STANDALONE)
  enable_testing()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

  # Build example directory.
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif()
