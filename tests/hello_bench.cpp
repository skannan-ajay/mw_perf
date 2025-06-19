#include <benchmark/benchmark.h>

static void bmStringCreation(benchmark::State& aState) {
  for (auto _ : aState)
    std::string emptyString;
}
// Register the function as a benchmark
BENCHMARK(bmStringCreation);

// Define another benchmark
static void bmStringCopy(benchmark::State& aState) {
  std::string x = "hello";
  for (auto _ : aState)
    std::string copy(x);
}
BENCHMARK(bmStringCopy);

BENCHMARK_MAIN();
