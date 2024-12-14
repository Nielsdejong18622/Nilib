# Define the static library Nilib
add_library(Nilib STATIC
    Color.cpp
    Window.cpp
)
# Link GLFW to the Nilib library
target_link_libraries(Nilib PUBLIC glfw opengl32)

# Include the headers
target_include_directories(Nilib PUBLIC
    ${CMAKE_SOURCE_DIR}/../include
)