#include "VNTable.hpp"

#include <benchmark/benchmark.h>
#include <random>

// Define the number of operations to perform in each benchmark
// This is used to control the size of the test data and the number of
// lookups//insertions performed during the benchmark. It can be adjusted based
// on the performance characteristics of the system being tested.
#define OP_COUNT 1e6

using TestTable = VNTable<int>;
std::default_random_engine engine;

// Sets up a VNTable as large as the range specified
// Uses VNTable's own insertOrReplace() method to fill it.
class TableFixture : public benchmark::Fixture {
public:
  void Setup(::benchmark::State& aState) {
    fTestTable = std::make_shared<TestTable>();
    auto vnDist = std::uniform_int_distribution<TestTable::ValueNumber>();
    for (int i = 0; i != aState.range(0); ++i) {
      auto valueNumber = vnDist(engine) % 200;
      fTestTable->insertOrReplace(i, valueNumber);
    }
  }

  void TearDown([[maybe_unused]] ::benchmark::State& aState) override {
    fTestTable->clear();
  }

  std::shared_ptr<TestTable> getTestTable() const {
    return fTestTable;
  }

private:
  std::shared_ptr<TestTable> fTestTable;
};

BENCHMARK_DEFINE_F(TableFixture, find)(benchmark::State& aState) {
  TableFixture::Setup(aState);
  auto testTable = TableFixture::getTestTable();

  // Test as many lookups specified by the range
  for (auto _ : aState) {
    for (int i = 0; i != OP_COUNT; ++i) {
      auto result = testTable->value(i);
      benchmark::DoNotOptimize(result);
    }
  }

  TableFixture::TearDown(aState);
  aState.SetComplexityN(aState.range(0));
}

BENCHMARK_DEFINE_F(TableFixture, insert)(benchmark::State& aState) {
  TableFixture::Setup(aState);
  auto testTable = TableFixture::getTestTable();

  // Test as many lookups specified by the range
  for (auto _ : aState) {
    for (int i = 0; i != OP_COUNT; ++i) {
      testTable->insertOrReplace(i * 1000, i % 200);
    }
  }

  TableFixture::TearDown(aState);
  aState.SetComplexityN(aState.range(0));
}

BENCHMARK_REGISTER_F(TableFixture, find)
    ->RangeMultiplier(10)
    ->Range(1e3, 1e6)
    ->Unit(benchmark::kMillisecond)
    ->Complexity();

BENCHMARK_REGISTER_F(TableFixture, insert)
    ->RangeMultiplier(10)
    ->Range(1e3, 1e6)
    ->Unit(benchmark::kMillisecond)
    ->Complexity();

BENCHMARK_MAIN();
