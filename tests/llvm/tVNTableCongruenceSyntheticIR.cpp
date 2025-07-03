#include "VNTable.hpp"

#include <benchmark/benchmark.h>
#include <boost/range.hpp>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <memory>
#include <numeric>
#include <tbb/parallel_for_each.h>
#include <vector>

using LlvmVNTable = VNTable<llvm::Value*>;

using namespace llvm;

// Global variable to store IR file path from command line
static std::string g_irFilePath = "./benchmarks/synthetic_heavy_gvn.ll";

class SyntheticIRFixture : public ::benchmark::Fixture {
protected:
  llvm::LLVMContext context_;
  std::unique_ptr<llvm::Module> module_;
  std::unique_ptr<LlvmVNTable> testTable_;
  std::vector<llvm::Value*> values_;

  void SetUp(const ::benchmark::State& state) override {
    // Load the synthetic IR file
    llvm::SMDiagnostic error;
    module_ = llvm::parseIRFile(g_irFilePath, error, context_);

    if (!module_) {
      ::benchmark::State& mutableState = const_cast<::benchmark::State&>(state);
      mutableState.SkipWithError("Failed to load synthetic_heavy_gvn.ll");
      return;
    }

    // Create VNTable and populate with values from the module
    testTable_ = std::make_unique<LlvmVNTable>();

    // Collect all values from the module
    for (auto& func : *module_) {
      // Add function arguments
      for (auto& arg : func.args()) {
        values_.push_back(&arg);
        testTable_->value(&arg); // Ensure value is in table
      }

      // Add all instructions
      for (auto& bb : func) {
        for (auto& inst : bb) {
          values_.push_back(&inst);
          testTable_->value(&inst); // Ensure value is in table
        }
      }
    }

    // Also add constants that appear in the module
    for (auto& func : *module_) {
      for (auto& bb : func) {
        for (auto& inst : bb) {
          for (auto& operand : inst.operands()) {
            if (auto* constant =
                    llvm::dyn_cast<llvm::Constant>(operand.get())) {
              if (std::find(values_.begin(), values_.end(), constant) ==
                  values_.end()) {
                values_.push_back(constant);
                testTable_->value(constant); // Ensure value is in table
              }
            }
          }
        }
      }
    }
  }

  void TearDown(const ::benchmark::State& state) override {
    values_.clear();
    testTable_.reset();
    module_.reset();
  }

public:
  LlvmVNTable* getTestTable() {
    return testTable_.get();
  }
  const std::vector<llvm::Value*>& getValues() const {
    return values_;
  }
};

// Benchmark congruence_naive implementation on synthetic IR
BENCHMARK_DEFINE_F(SyntheticIRFixture,
                   congruence_naive)(benchmark::State& aState) {
  auto testTable = getTestTable();
  const auto& values = getValues();

  if (values.empty()) {
    aState.SkipWithError("No values loaded from synthetic IR");
    return;
  }

  for (auto _ : aState) {
    // Test congruence operations on random value numbers from the synthetic IR
    for (size_t i = 0; i < 1000000; ++i) {
      auto optVN = testTable->value(values[i % values.size()]);
      if (!optVN)
        continue;
      auto vn = *optVN;
      auto result = testTable->congruence(vn);

      for (const auto& [entity, entityVN] :
           boost::make_iterator_range(result)) {
        auto l = entity;
        auto r = entityVN;
        ::benchmark::DoNotOptimize(l);
        ::benchmark::DoNotOptimize(r);
      }
    }
  }

  aState.SetItemsProcessed(aState.iterations() * 1000000);
}

// Benchmark congruence_uniquify_naive implementation on synthetic IR
BENCHMARK_DEFINE_F(SyntheticIRFixture,
                   congruence_uniquify_naive)(benchmark::State& aState) {
  auto testTable = getTestTable();
  const auto& values = getValues();

  if (values.empty()) {
    aState.SkipWithError("No values loaded from synthetic IR");
    return;
  }

  for (auto _ : aState) {
    for (size_t i = 0; i < 1000000; ++i) {
      auto optVN = testTable->value(values[i % values.size()]);
      if (!optVN)
        continue;
      auto vn = *optVN;
      auto result = testTable->congruence(vn);

      for (const auto& [entity, entityVN] :
           boost::make_iterator_range(result)) {
        auto l = entity;
        auto r = entityVN;
        ::benchmark::DoNotOptimize(l);
        ::benchmark::DoNotOptimize(r);
      }
    }
  }

  aState.SetItemsProcessed(aState.iterations() * 1000000);
}

// Benchmark congruence_parallel_results_only implementation on synthetic IR
BENCHMARK_DEFINE_F(SyntheticIRFixture,
                   congruence_parallel_results_only)(benchmark::State& aState) {
  auto testTable = getTestTable();
  const auto& values = getValues();

  if (values.empty()) {
    aState.SkipWithError("No values loaded from synthetic IR");
    return;
  }

  for (auto _ : aState) {
    for (size_t i = 0; i < 1000000; ++i) {
      auto optVN = testTable->value(values[i % values.size()]);
      if (!optVN)
        continue;
      auto vn = *optVN;
      auto result = testTable->congruence(vn);

      for (const auto& [entity, entityVN] :
           boost::make_iterator_range(result)) {
        auto l = entity;
        auto r = entityVN;
        ::benchmark::DoNotOptimize(l);
        ::benchmark::DoNotOptimize(r);
      }
    }
  }

  aState.SetItemsProcessed(aState.iterations() * 1000000);
}

// Benchmark congruence_fast implementation on synthetic IR
BENCHMARK_DEFINE_F(SyntheticIRFixture,
                   congruence_fast)(benchmark::State& aState) {
  auto testTable = getTestTable();
  const auto& values = getValues();

  if (values.empty()) {
    aState.SkipWithError("No values loaded from synthetic IR");
    return;
  }

  for (auto _ : aState) {
    for (size_t i = 0; i < 1000000; ++i) {
      auto optVN = testTable->value(values[i % values.size()]);
      if (!optVN)
        continue;
      auto vn = *optVN;
      auto result = testTable->congruence(vn);

      for (const auto& [entity, entityVN] :
           boost::make_iterator_range(result)) {
        auto l = entity;
        auto r = entityVN;
        ::benchmark::DoNotOptimize(l);
        ::benchmark::DoNotOptimize(r);
      }
    }
  }

  aState.SetItemsProcessed(aState.iterations() * 1000000);
}

// Benchmark congruence_parallel_lookup implementation on synthetic IR
BENCHMARK_DEFINE_F(SyntheticIRFixture,
                   congruence_parallel_lookup)(benchmark::State& aState) {
  auto testTable = getTestTable();
  const auto& values = getValues();

  if (values.empty()) {
    aState.SkipWithError("No values loaded from synthetic IR");
    return;
  }

  for (auto _ : aState) {
    // Create indices for parallel processing
    std::vector<size_t> indices(1000000);
    std::iota(indices.begin(), indices.end(), 0);

    // Parallelize the congruence lookups using TBB
    tbb::parallel_for_each(indices.begin(), indices.end(), [&](size_t i) {
      auto optVN = testTable->value(values[i % values.size()]);
      if (!optVN)
        return;
      auto vn = *optVN;
      auto result = testTable->congruence(vn);

      for (const auto& [entity, entityVN] :
           boost::make_iterator_range(result)) {
        auto l = entity;
        auto r = entityVN;
        ::benchmark::DoNotOptimize(l);
        ::benchmark::DoNotOptimize(r);
      }
    });
  }

  aState.SetItemsProcessed(aState.iterations() * 1000000);
}

BENCHMARK_REGISTER_F(SyntheticIRFixture, congruence_naive)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_REGISTER_F(SyntheticIRFixture, congruence_uniquify_naive)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_REGISTER_F(SyntheticIRFixture, congruence_parallel_results_only)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_REGISTER_F(SyntheticIRFixture, congruence_fast)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_REGISTER_F(SyntheticIRFixture, congruence_parallel_lookup)
    ->Unit(benchmark::kMillisecond);

int main(int argc, char** argv) {
  ::benchmark::Initialize(&argc, argv);

  // Check for IR file path argument
  if (argc > 1) {
    g_irFilePath = argv[1];
    ::benchmark::AddCustomContext("IRFilePath", argv[1]);
  }

  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return 0;
}
