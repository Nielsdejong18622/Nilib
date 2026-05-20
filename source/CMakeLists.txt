# Find all .cpp and .hpp files
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Define the static library Nilib
add_library(Nilib STATIC ${CPP_FILES})

# Link GLFW, Glad, HiGHS, Gurobi, OpenGL
if(WIN32)
    if (HIGHS)
        target_link_libraries(Nilib PUBLIC highs::highs)
    endif()
    if (GUROBI)
        target_include_directories(Nilib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/gurobi1103/include)
        target_link_libraries(Nilib PUBLIC gurobi_c++mt ${GUROBI_LIBRARY})
    endif()
    if (GLFW)
        target_link_libraries(Nilib PUBLIC glfw glad opengl32)
    endif()
elseif(LINUX)
    if (HIGHS)
        target_link_libraries(Nilib PUBLIC highs::highs)
    endif()
    if (GLFW)
        find_package(OpenGL REQUIRED)
        target_link_libraries(Nilib PUBLIC glfw glad OpenGL::GL)
    endif()
endif()

# Include headers
target_include_directories(Nilib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)