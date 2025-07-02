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
add_executable(tVNTable tVNTable.cpp)
add_executable(tStdMap tStdMap.cpp)
add_executable(tStdMultiMap tStdMultiMap.cpp)
add_executable(tVNTableCongruence tVNTableCongruence.cpp)

# Add libraries to link against
target_link_libraries(hello_test GTest::gtest_main ${Boost_LIBRARIES})
target_link_libraries(tVNTableCongruence GTest::gtest_main benchmark::benchmark ${Boost_LIBRARIES} TBB::tbb)
target_link_libraries(tLinearComplexityExample GTest::gtest_main benchmark::benchmark ${Boost_LIBRARIES})
target_link_libraries(tLogarithmicComplexityExample GTest::gtest_main benchmark::benchmark ${Boost_LIBRARIES})
target_link_libraries(tVNTable GTest::gtest_main benchmark::benchmark ${Boost_LIBRARIES} TBB::tbb)
target_link_libraries(tStdMap GTest::gtest_main benchmark::benchmark ${Boost_LIBRARIES})
target_link_libraries(tStdMultiMap GTest::gtest_main benchmark::benchmark ${Boost_LIBRARIES})

include(GoogleTest)

# Test discovery
gtest_discover_tests(hello_test)
#gtest_discover_tests(hello_bench)

add_subdirectory(llvm)
