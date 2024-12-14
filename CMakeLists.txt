cmake_minimum_required(VERSION 3.10)
project(Nilib VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the external GLFW submodule
add_subdirectory(external/glfw)

# Add subdirectories for the Nilibrary
add_subdirectory(source)
add_subdirectory(tests)
