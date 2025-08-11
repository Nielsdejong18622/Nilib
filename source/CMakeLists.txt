# Find all .cpp and .hpp files
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Define the static library Nilib
add_library(Nilib STATIC ${CPP_FILES})

# target_compile_options(Nilib PRIVATE -Wextra -Ofast)

# Link GLFW to the Nilib library
if(WIN32)
    target_link_libraries(Nilib PUBLIC glfw glad gurobi_c++mt ${GUROBI_LIBRARY} opengl32)
elseif(LINUX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Nilib PUBLIC glfw glad OpenGL::GL)
endif()

# Include the headers
target_include_directories(Nilib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/gurobi1103/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/stb/
)

# Precompiled header.
target_precompile_headers(Nilib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Nilib/Nilibpch.h)