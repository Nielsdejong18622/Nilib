cmake_minimum_required(VERSION 3.10)

project(GLAD_PROJECT C)


include_directories()

# Define the static library Nilib
add_library(glad STATIC src/glad.c)
# Link GLFW to the Nilib library

# Include the headers
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
