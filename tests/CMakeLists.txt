# Setup googletest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Setup google benchmark
find_package(benchmark REQUIRED)

include_directories(/opt/homebrew/Cellar/google-benchmark/1.9.4/include/)
link_directories(/opt/homebrew/Cellar/google-benchmark/1.9.4/lib)

enable_testing()

# Add targets
add_executable(hello_test hello_test.cpp)
add_executable(hello_bench hello_bench.cpp)
add_executable(tLinearComplexityExample tLinearComplexityExample.cpp)
add_executable(tLogarithmicComplexityExample tLogarithmicComplexityExample.cpp)
add_executable(tMemoryManagerExample tMemoryManagerExample.cpp)


# Add libraries to link against
target_link_libraries(hello_test GTest::gtest_main)
target_link_libraries(hello_bench GTest::gtest_main benchmark::benchmark)
target_link_libraries(tLinearComplexityExample GTest::gtest_main benchmark::benchmark)
target_link_libraries(tLogarithmicComplexityExample GTest::gtest_main benchmark::benchmark)

target_include_directories(tMemoryManagerExample PRIVATE /opt/homebrew/Cellar/gperftools/2.16/include)
target_link_directories(tMemoryManagerExample PRIVATE /opt/homebrew/Cellar/gperftools/2.16/lib)
target_link_libraries(tMemoryManagerExample PRIVATE GTest::gtest_main benchmark::benchmark tcmalloc)

include(GoogleTest)

# Test discovery
gtest_discover_tests(hello_test)
#gtest_discover_tests(hello_bench)
