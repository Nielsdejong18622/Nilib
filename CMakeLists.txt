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

if (NILIB_BUILD_TESTING)
  enable_testing()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
 endif()
