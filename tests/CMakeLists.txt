# Define the test executable
add_executable(TestVersion TestVersion.cpp)
add_executable(TestGlad TestGlad.cpp)
add_executable(TestTriangle TestTriangle.cpp)

# Link the static library to the test executable
target_link_libraries(TestVersion PUBLIC Nilib)
target_link_libraries(TestGlad PUBLIC Nilib)
target_link_libraries(TestTriangle PUBLIC Nilib)

# Execute the test programs. 
add_test(NAME DisplayVersion COMMAND TestVersion)
add_test(NAME DisplayGladVersion COMMAND TestGlad)
add_test(NAME DisplayTriangle COMMAND TestTriangle)
