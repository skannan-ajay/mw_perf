#include <algorithm>                // std::max, std::min
#include <benchmark/benchmark.h>    // link benchmark
#include <cstring>                  // malloc
#include <gperftools/malloc_hook.h> // link tcmalloc

benchmark::IterationCount g_num_new = 0;
benchmark::IterationCount g_sum_size_new = 0;
benchmark::IterationCount g_max_size_new = 0;
benchmark::IterationCount g_min_size_new = -1;
auto new_hook = [](const void*, size_t aSize) {
  ++g_num_new;
  g_sum_size_new += static_cast<benchmark::IterationCount>(aSize);
  g_max_size_new = std::max(static_cast<size_t>(g_max_size_new), aSize);
  g_min_size_new = std::min(static_cast<size_t>(g_min_size_new), aSize);
};

#define BEFORE_TEST                                        \
  benchmark::IterationCount num_new = g_num_new;           \
  benchmark::IterationCount sum_size_new = g_sum_size_new; \
  g_max_size_new = 0;                                      \
  g_min_size_new = -1;                                     \
  MallocHook::AddNewHook(new_hook);

#define AFTER_TEST                                                      \
  MallocHook::RemoveNewHook(new_hook);                                  \
  auto iter = state.iterations();                                       \
  state.counters["#new"] = (g_num_new - num_new) / iter;                \
  state.counters["sum_new_B"] = (g_sum_size_new - sum_size_new) / iter; \
  state.counters["avg_new_B"] =                                         \
      (g_sum_size_new - sum_size_new) / (g_num_new - num_new);          \
  state.counters["max_new_B"] = g_max_size_new;                         \
  if (((benchmark::IterationCount) - 1) != g_min_size_new) {            \
    state.counters["min_new_B"] = g_min_size_new;                       \
  }

class TestMemoryManager : public benchmark::MemoryManager {
  void Start() override {
  }
  void Stop(Result& result) override {
    result.num_allocs = 42;
    result.max_bytes_used = 42000;
  }
};

static void bmMemoryUsageReport(benchmark::State& state) {
  // BEFORE_TEST
  for (auto _ : state) {
    void* ret1 = malloc(state.range(0));
    void* ret2 = malloc(state.range(1));
    void* ret3 = malloc(state.range(2));
    free(ret1);
    free(ret2);
    free(ret3);
  }
  // AFTER_TEST
}
BENCHMARK(bmMemoryUsageReport)->Args({32, 128, 32});
BENCHMARK(bmMemoryUsageReport)->Args({320, 640, 960});

int main(int argc, char* argv[]) {
  benchmark::MaybeReenterWithoutASLR(argc, argv);
  std::unique_ptr<benchmark::MemoryManager> mm(new TestMemoryManager());

  benchmark::RegisterMemoryManager(mm.get());
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  benchmark::RegisterMemoryManager(nullptr);
}
