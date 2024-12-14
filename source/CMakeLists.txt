# Define the static library Nilib
add_library(Nilib STATIC
    Color.cpp
    Window.cpp
)

# Include the headers
target_include_directories(Nilib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Link GLFW to the Nilib library
target_link_libraries(Nilib PRIVATE glfw)
