# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(GurobiCPP)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler options
if (MSVC)
    add_definitions(-DWIN64)
    add_compile_options(/EHsc /MT /nologo)
endif()

# Source directory
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cpp)

# Source files
set(SOURCES
    ${SRC_DIR}/Env.cpp
    ${SRC_DIR}/Model.cpp
    ${SRC_DIR}/attrprivate.cpp
    ${SRC_DIR}/Var.cpp
    ${SRC_DIR}/Constr.cpp
    ${SRC_DIR}/LinExpr.cpp
    ${SRC_DIR}/QuadExpr.cpp
    ${SRC_DIR}/Exception.cpp
    ${SRC_DIR}/Callback.cpp
    ${SRC_DIR}/Column.cpp
    ${SRC_DIR}/SOS.cpp
    ${SRC_DIR}/QConstr.cpp
    ${SRC_DIR}/GenConstr.cpp
    ${SRC_DIR}/TempConstr.cpp
    ${SRC_DIR}/Batch.cpp
)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Create a static library
add_library(gurobi_c++mt STATIC ${SOURCES})

# Clean target
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "gurobi_c++mt.lib")
