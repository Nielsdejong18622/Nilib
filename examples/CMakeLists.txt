# Define the example executables
add_executable(ExampleTriangle Triangle.cpp)

# Link the static library to the example executable
target_link_libraries(ExampleTriangle PUBLIC Nilib)
