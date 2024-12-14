# Define the test executable
add_executable(test_Nilib
    Version.cpp
)

# Link the static library to the test executable
target_link_libraries(test_Nilib PRIVATE Nilib glfw)

# Set C++ flags for testing (e.g., for debugging)
target_compile_options(test_Nilib PRIVATE -g -Wall)

# Add a custom target to run the tests
add_custom_target(run_tests
    COMMAND test_Nilib
    DEPENDS test_Nilib
    COMMENT "Running Nilib tests"
)
