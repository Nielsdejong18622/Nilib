# Define the test executable
add_executable(Version Version.cpp)
add_executable(GladTest Glad.cpp)
add_executable(TriangleTest Triangle.cpp)

# Link the static library to the test executable
target_link_libraries(Version PUBLIC Nilib)
target_link_libraries(GladTest PUBLIC Nilib)
target_link_libraries(TriangleTest PUBLIC Nilib)

# Execute the test programs. 
add_test(NAME DisplayVersion COMMAND Version)
add_test(NAME DisplayGladVersion COMMAND GladTest)
add_test(NAME DisplayTriangle COMMAND TriangleTest)
