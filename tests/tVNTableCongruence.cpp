#include "VNTable.hpp"

#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>
#include <random>
#include <tbb/parallel_for_each.h>
#include <vector>

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

// Naive approach - Sequential lookup, Sequential processing
BENCHMARK_DEFINE_F(TableFixture, congruence_naive)(benchmark::State& aState) {
  TableFixture::Setup(aState);
  auto testTable = TableFixture::getTestTable();

  for (auto _ : aState) {
    for (int i = 0; i != OP_COUNT; ++i) {
      auto result = testTable->congruence(i % 200);
      for (const auto& [entity, vn] : boost::make_iterator_range(result)) {
        auto l = entity;
        auto r = vn;
        ::benchmark::DoNotOptimize(l);
        ::benchmark::DoNotOptimize(r);
      }
    }
  }

  TableFixture::TearDown(aState);
  aState.SetComplexityN(aState.range(0));
}

// Same as above except that keys to lookup are prematurely collected and
// uniquified
BENCHMARK_DEFINE_F(TableFixture,
                   congruence_uniquify_naive)(benchmark::State& aState) {
  TableFixture::Setup(aState);
  auto testTable = TableFixture::getTestTable();

  for (auto _ : aState) {
    // Collect all the distinct keys (value numbers) sequentially
    std::set<TestTable::ValueNumber> keys;
    for (int i = 0; i != OP_COUNT; ++i) {
      keys.insert(i % 200);
    }

    // Now compute the congruence for the keys (value numbers)
    std::for_each(
        keys.begin(), keys.end(), [&](const TestTable::ValueNumber& aValue) {
          auto congruence = testTable->congruence(aValue);
          ::benchmark::DoNotOptimize(congruence);
          for (const auto& [entity, vn] :
               boost::make_iterator_range(congruence)) {
            // Do something with the entity/vn
            auto l = entity;
            auto r = vn;
            ::benchmark::DoNotOptimize(l);
            ::benchmark::DoNotOptimize(r);
          }
        });
  }

  TableFixture::TearDown(aState);
  aState.SetComplexityN(aState.range(0));
}

// Sequential lookup, Parallel processing of results
BENCHMARK_DEFINE_F(TableFixture,
                   congruence_parallel_results_only)(benchmark::State& aState) {
  TableFixture::Setup(aState);
  auto testTable = TableFixture::getTestTable();

  for (auto _ : aState) {
    for (int i = 0; i != OP_COUNT; ++i) {
      auto result = testTable->congruence(i % 200);

      // Collect the results sequentially
      std::set<int> values;
      for (const auto& [entity, vn] : boost::make_iterator_range(result)) {
        values.insert(entity);
      }

      // Now parallelize processing with Intel TBB
      tbb::parallel_for_each(
          values.begin(), values.end(), [](const int& aValue) {
            // Do something with this entity (expr)
            auto testVal = aValue * 2;
            ::benchmark::DoNotOptimize(testVal);
          });
    }
  }

  TableFixture::TearDown(aState);
  aState.SetComplexityN(aState.range(0));
}

// Parallel lookup/processing over unique keys (value numbers)
BENCHMARK_DEFINE_F(TableFixture, congruence_fast)(benchmark::State& aState) {
  TableFixture::Setup(aState);
  auto testTable = TableFixture::getTestTable();

  for (auto _ : aState) {
    // Collect all the distinct keys (value numbers) sequentially
    std::set<TestTable::ValueNumber> keys;
    for (int i = 0; i != OP_COUNT; ++i) {
      keys.insert(i % 200);
    }

    // Now parallely compute the congruence for the value numbers
    tbb::parallel_for_each(
        keys.begin(), keys.end(), [&](const TestTable::ValueNumber& aValue) {
          auto congruence = testTable->congruence(aValue);
          ::benchmark::DoNotOptimize(congruence);
          for (const auto& [entity, vn] :
               boost::make_iterator_range(congruence)) {
            // Do something with the entity/vn
            auto l = entity;
            auto r = vn;
            ::benchmark::DoNotOptimize(l);
            ::benchmark::DoNotOptimize(r);
          }
        });
  }

  TableFixture::TearDown(aState);
  aState.SetComplexityN(aState.range(0));
}

// Parallelize the outermost benchmarking loop (operational loop)
BENCHMARK_DEFINE_F(TableFixture,
                   congruence_parallel_lookup)(benchmark::State& aState) {
  TableFixture::Setup(aState);
  auto testTable = TableFixture::getTestTable();

  for (auto _ : aState) {
    // Create a vector of indices to parallelize over
    std::vector<int> indices(OP_COUNT);
    std::iota(indices.begin(), indices.end(), 0);
    
    // Parallelize the OP_COUNT lookups using TBB
    tbb::parallel_for_each(
        indices.begin(), indices.end(), [&](int i) {
          auto result = testTable->congruence(i % 200);
          for (const auto& [entity, vn] : boost::make_iterator_range(result)) {
            auto l = entity;
            auto r = vn;
            ::benchmark::DoNotOptimize(l);
            ::benchmark::DoNotOptimize(r);
          }
        });
  }

  TableFixture::TearDown(aState);
  aState.SetComplexityN(aState.range(0));
}

BENCHMARK_REGISTER_F(TableFixture, congruence_naive)
    ->RangeMultiplier(10)
    ->Range(1e3, 1e5)
    ->Unit(benchmark::kMillisecond)
    ->Complexity();

BENCHMARK_REGISTER_F(TableFixture, congruence_uniquify_naive)
    ->RangeMultiplier(10)
    ->Range(1e3, 1e5)
    ->Unit(benchmark::kMillisecond)
    ->Complexity();

BENCHMARK_REGISTER_F(TableFixture, congruence_parallel_results_only)
    ->RangeMultiplier(10)
    ->Range(1e3, 1e5)
    ->Unit(benchmark::kMillisecond)
    ->Complexity();

BENCHMARK_REGISTER_F(TableFixture, congruence_fast)
    ->RangeMultiplier(10)
    ->Range(1e3, 1e5)
    ->Unit(benchmark::kMillisecond)
    ->Complexity();

BENCHMARK_REGISTER_F(TableFixture, congruence_parallel_lookup)
    ->RangeMultiplier(10)
    ->Range(1e3, 1e5)
    ->Unit(benchmark::kMillisecond)
    ->Complexity();

BENCHMARK_MAIN();
