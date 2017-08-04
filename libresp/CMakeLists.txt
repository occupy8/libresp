cmake_minimum_required(VERSION 2.8)

project(test CXX C)

set (PROJECT_VERSION_MAJOR 1) 
set (PROJECT_VERSION_MINOR 0)
set (PROJECT_VERSION_PATCH 0)

include_directories(${PROJECT_SOURCE_DIR}/include/libresp/)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/libresp/ DIR_SRCS)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include(${PROJECT_SOURCE_DIR}/cmake/UserOption.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CheckCompiler.cmake)

include(${PROJECT_SOURCE_DIR}/package/package.cmake)

add_executable(./test ${DIR_SRCS})
