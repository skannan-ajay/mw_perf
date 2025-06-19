#include "LinearSearch.hpp"

#include <benchmark/benchmark.h>
#include <random>

using ContainerType = std::vector<int>;

class ContainerFixture : public benchmark::Fixture {
public:
  void Setup(::benchmark::State& aState) {
    for (int i = 0; i < aState.range(0); ++i) {
      fData.push_back(i);
    }
  }
  void TearDown([[maybe_unused]] ::benchmark::State& aState) override {
    fData.clear();
  }

  std::shared_ptr<std::vector<int>> getData() {
    return std::make_shared<ContainerType>(fData);
  }

private:
  ContainerType fData;
};

BENCHMARK_DEFINE_F(ContainerFixture, bmLinearSearch)(benchmark::State& aState) {
  ContainerFixture::Setup(aState);
  auto data = ContainerFixture::getData();
  LinearSearch<ContainerType> searcher(data);

  static std::default_random_engine engine;
  auto dist = std::uniform_int_distribution<int>(
      static_cast<int>(aState.range(0)), static_cast<int>(aState.range(0)) * 2);
  auto valToSearch = dist(engine);

  for (auto _ : aState) {
    auto result = searcher.rank(valToSearch);
    benchmark::DoNotOptimize(result);
  }

  ContainerFixture::TearDown(aState);
  aState.SetComplexityN(aState.range(0));
}

BENCHMARK_REGISTER_F(ContainerFixture, bmLinearSearch)
    ->RangeMultiplier(10)
    ->Range(1e3, 1e6)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();
BENCHMARK_MAIN();
