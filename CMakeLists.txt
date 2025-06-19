cmake_minimum_required(VERSION 4.0)
project(mw_perf)

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a)

set(CMAKE_CXX_FLAGS -Wall)
set(CMAKE_BUILD_TYPE Release) 

# include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# build directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(lib)
add_subdirectory(tests)

