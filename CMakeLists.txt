cmake_minimum_required(VERSION 3.10)
project(Nilib VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Add the external GLFW submodule
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/gurobi1103)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glad)

# Add subdirectories for the Nilibrary
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source)

string(COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" NILIB_STANDALONE)
if (NILIB_STANDALONE)
  enable_testing()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

  # Build example directory.
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
 endif()
