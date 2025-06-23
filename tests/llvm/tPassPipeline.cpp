#include <benchmark/benchmark.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/CommandLine.h>

using namespace llvm;

static cl::opt<std::string>
    inputFileName(cl::Positional, cl::desc("<input bitcode>"), cl::init("-"));

// Declare context and module in the right construction order.
std::unique_ptr<LLVMContext> ctx;
std::unique_ptr<Module> module;

struct AnalysisManagers {
  LoopAnalysisManager fLoopAnalysisManager;
  FunctionAnalysisManager fFunctionAnalysisManager;
  ModuleAnalysisManager fModuleAnalysisManager;
  CGSCCAnalysisManager fCGSCCAnalysisManager;
};

static void registerAnalysesOnPassBuilder(PassBuilder& aPassBuilder,
                                          AnalysisManagers& aAnalysesManagers) {
  aPassBuilder.registerModuleAnalyses(aAnalysesManagers.fModuleAnalysisManager);
  aPassBuilder.registerCGSCCAnalyses(aAnalysesManagers.fCGSCCAnalysisManager);
  aPassBuilder.registerFunctionAnalyses(
      aAnalysesManagers.fFunctionAnalysisManager);
  aPassBuilder.registerLoopAnalyses(aAnalysesManagers.fLoopAnalysisManager);
  aPassBuilder.crossRegisterProxies(aAnalysesManagers.fLoopAnalysisManager,
                                    aAnalysesManagers.fFunctionAnalysisManager,
                                    aAnalysesManagers.fCGSCCAnalysisManager,
                                    aAnalysesManagers.fModuleAnalysisManager);
}

static void
llvmPassPipeline(benchmark::State& aState,
                 OptimizationLevel aOptLevel = OptimizationLevel::O2) {
  // Initialize Pass Builder
  PassBuilder passBuilder;
  AnalysisManagers analysesManagers;
  registerAnalysesOnPassBuilder(passBuilder, analysesManagers);

  for (auto _ : aState) {
    // Initialize Module Pass Manager
    ModulePassManager mpm =
        passBuilder.buildPerModuleDefaultPipeline(aOptLevel);
    mpm.run(*module, analysesManagers.fModuleAnalysisManager);
  }
}

// Register the benchmarks for each optimization level.
BENCHMARK_CAPTURE(llvmPassPipeline, OptLevelO0, OptimizationLevel::O0)
    ->Complexity()
    ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(llvmPassPipeline, OptLevelO1, OptimizationLevel::O1)
    ->Complexity()
    ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(llvmPassPipeline, OptLevelO2, OptimizationLevel::O2)
    ->Complexity()
    ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(llvmPassPipeline, OptLevelO3, OptimizationLevel::O3)
    ->Complexity()
    ->Unit(benchmark::kMillisecond);

int main(int argc, char** argv) {
  // Create context and parse the input bitcode file
  cl::ParseCommandLineOptions(argc, argv, "LLVM System Compiler\n");
  ctx = std::make_unique<LLVMContext>();
  SMDiagnostic diag;

  module = parseIRFile(inputFileName, diag, *ctx);
  if (!module) {
    diag.print(argv[0], errs());
    return 1;
  }

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  benchmark::Shutdown();

  return 0;
}
