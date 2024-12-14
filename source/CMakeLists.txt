# Define the static library Nilib
add_library(Nilib STATIC
    Color.cpp
    Window.cpp
)
# Link GLFW to the Nilib library

target_link_libraries(Nilib PUBLIC glfw opengl32 gurobi_c++mt ${GUROBI_LIBRARY} glad)

# Include the headers
target_include_directories(Nilib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/gurobi1103/include
)
