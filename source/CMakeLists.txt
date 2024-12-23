# Find all .cpp and .hpp files
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Define the static library Nilib
add_library(Nilib STATIC ${CPP_FILES})

# Add compiler definition only when building Nilib as stand-alone.
target_compile_definitions(Nilib PRIVATE NILIB_BUILD_TESTING)

# Link GLFW to the Nilib library
target_link_libraries(Nilib PUBLIC glfw opengl32 gurobi_c++mt ${GUROBI_LIBRARY} glad)

# Include the headers
target_include_directories(Nilib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/gurobi1103/include
)

# Precompiled header.
target_precompile_headers(Nilib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Nilib/Nilibpch.h)