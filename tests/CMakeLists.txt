# Define the test executable
add_executable(test_Nilib
    Version.cpp
)
# Link the static library to the test executable
target_link_libraries(test_Nilib PUBLIC Nilib)

# Set C++ flags for testing (e.g., for debugging)
target_compile_options(test_Nilib PUBLIC -g -Wall)

# Execute the test programs. 
add_test(NAME DisplayVersion COMMAND test_Nilib)
