# Define the example executables
add_executable(Triangle Triangle.cpp)

# Link the static library to the example executable
target_link_libraries(Triangle PUBLIC Nilib)
