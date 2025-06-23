#include <benchmark/benchmark.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/InitializePasses.h>
#include <llvm/LinkAllPasses.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <memory>

using namespace llvm;

enum Pass { DCE, LICM, SROA, GVN, INST_COMBINE, MEM2REG };

static cl::opt<std::string>
    inputFileName(cl::Positional, cl::desc("<input bitcode>"), cl::init("-"));
std::unique_ptr<LLVMContext> ctx;
std::unique_ptr<Module> module;

static void llvmOptBenchmark(benchmark::State& aState, const ::Pass& aPass) {
  legacy::PassManager passManager;
  switch (aPass) {
    case ::Pass::DCE:
      passManager.add(createDeadCodeEliminationPass());
      break;
    case ::Pass::LICM:
      passManager.add(createLICMPass());
      break;
    case ::Pass::SROA:
      passManager.add(createSROAPass());
      break;
    case ::Pass::GVN:
      passManager.add(createGVNPass());
      break;
    case ::Pass::INST_COMBINE:
      passManager.add(createInstructionCombiningPass());
      break;
    case ::Pass::MEM2REG:
      passManager.add(createPromoteMemoryToRegisterPass());
      break;
  }

  for (auto _ : aState) {
    passManager.run(*module);
  }
}

// Register the benchmark
BENCHMARK_CAPTURE(llvmOptBenchmark, DeadCodeElimination, ::Pass::DCE)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(llvmOptBenchmark, LICM, ::Pass::LICM)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(llvmOptBenchmark, SROA, ::Pass::SROA)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(llvmOptBenchmark, GVN, ::Pass::GVN)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(llvmOptBenchmark, InstCombine, ::Pass::INST_COMBINE)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(llvmOptBenchmark, Mem2Reg, ::Pass::MEM2REG)
    ->Unit(benchmark::kMillisecond);

int main(int argc, char** argv) {
  // Initialize passes
  PassRegistry& passRegistry = *PassRegistry::getPassRegistry();
  initializeCore(passRegistry);
  initializeScalarOpts(passRegistry);
  initializeAnalysis(passRegistry);
  initializeTransformUtils(passRegistry);

  cl::ParseCommandLineOptions(argc, argv, "LLVM System Compiler\n");

  // Create context and parse the input bitcode file
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
