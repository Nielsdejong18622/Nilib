# Define the example executables
add_executable(ExampleTriangle Triangle.cpp)
add_executable(ExampleNN_XOR NeuralNet_XOR.cpp)
add_executable(ExampleNN_SUM NeuralNet_Sum.cpp)

# Link the static library to the example executable
target_link_libraries(ExampleTriangle PUBLIC Nilib)
target_link_libraries(ExampleNN_XOR PUBLIC Nilib)
target_link_libraries(ExampleNN_SUM PUBLIC Nilib)
