# Define the example executables
add_executable(Triangle Triangle.cpp)
add_executable(NeuralNet_XOR NeuralNet_XOR.cpp)
add_executable(NeuralNet_Sum NeuralNet_Sum.cpp)
add_executable(Graph Graph.cpp)
add_executable(Profiler Profiler.cpp)
add_executable(Logger Logger.cpp)
add_executable(MultipleWindows MultipleWindows.cpp)
add_executable(LinAlg LinAlg.cpp)
add_executable(RNG2 RNG.cpp)

target_compile_options(LinAlg PRIVATE -Wextra -Ofast)

# Link the static library to the example executable
target_link_libraries(Triangle PUBLIC Nilib)
target_link_libraries(NeuralNet_XOR PUBLIC Nilib)
target_link_libraries(NeuralNet_Sum PUBLIC Nilib)
target_link_libraries(Graph PUBLIC Nilib)
target_link_libraries(Profiler PUBLIC Nilib)
target_link_libraries(Logger PUBLIC Nilib)
target_link_libraries(MultipleWindows PUBLIC Nilib)
target_link_libraries(LinAlg PUBLIC Nilib)
target_link_libraries(RNG2 PUBLIC Nilib)
