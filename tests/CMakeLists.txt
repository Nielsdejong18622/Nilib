# Define the test executable
add_executable(Version Version.cpp)
add_executable(GladTest Glad.cpp)

# Link the static library to the test executable
target_link_libraries(Version PUBLIC Nilib)
target_link_libraries(GladTest PUBLIC Nilib)

# Set C++ flags for testing (e.g., for debugging)
target_compile_options(Version PUBLIC -g -Wall)
target_compile_options(GladTest PUBLIC -g -Wall)

# Execute the test programs. 
add_test(NAME DisplayVersion COMMAND Version)
add_test(NAME DisplayGladVersion COMMAND GladTest)
