#zlib
set(ZLIB_ROOT /opt/homebrew/Cellar/zlib/1.3.1)
find_package(ZLIB REQUIRED)

# zstd
set(ZSTD_ROOT /opt/homebrew/opt/zstd)
find_library(ZSTD_LIBRARY NAMES zstd PATHS ${ZSTD_ROOT}/lib REQUIRED)
include_directories(${ZSTD_ROOT}/include)

# llvm
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(/opt/homebrew/opt/llvm/include)
link_directories(/opt/homebrew/opt/llvm/lib)

#libc++
link_directories(/opt/homebrew/opt/llvm/lib/c++)

#libunwind
#link_directories(/opt/homebrew/opt/llvm/lib/unwind)

# add the executable
add_executable(tDeadCodeElimination tDeadCodeElimination.cpp)

llvm_map_components_to_libnames(llvm_libs support core irreader passes analysis transformutils scalaropts)

target_link_libraries(tDeadCodeElimination
  GTest::gtest_main
  benchmark::benchmark
  ${llvm_libs}
  ${ZSTD_LIBRARY}
)
