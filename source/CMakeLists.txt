# Find all .cpp and .hpp files
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Define the static library Nilib
add_library(Nilib STATIC ${CPP_FILES})

# Link GLFW, Glad, HiGHS, Gurobi, OpenGL
if(WIN32)
    target_link_libraries(Nilib PUBLIC glfw glad highs::highs gurobi_c++mt ${GUROBI_LIBRARY} opengl32)
elseif(LINUX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Nilib PUBLIC glfw glad highs::highs OpenGL::GL)
endif()

# Include headers
target_include_directories(Nilib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/gurobi1103/include
)