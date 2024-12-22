# Define the example executables
add_executable(Triangle Triangle.cpp)
add_executable(NeuralNet_XOR NeuralNet_XOR.cpp)
add_executable(NeuralNet_Sum NeuralNet_Sum.cpp)

# Link the static library to the example executable
target_link_libraries(Triangle PUBLIC Nilib)
target_link_libraries(NeuralNet_XOR PUBLIC Nilib)
target_link_libraries(NeuralNet_Sum PUBLIC Nilib)
