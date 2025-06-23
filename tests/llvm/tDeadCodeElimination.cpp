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

static cl::opt<std::string>
    inputFileName(cl::Positional, cl::desc("<input bitcode>"), cl::init("-"));
std::unique_ptr<Module> originalModule;
std::unique_ptr<LLVMContext> globalCtx;

static void llvmDeadCodeElimination(benchmark::State &aState) {
  // Create a legacy pass manager
  legacy::PassManager passManager;

  // Add the dead code elimination pass
  passManager.add(createDeadCodeEliminationPass());
  passManager.add(createPromoteMemoryToRegisterPass());
  passManager.add(createLICMPass());

  // Run the pass manager on cloned modules to avoid modifying the original
  for (auto _ : aState) {
    auto moduleClone = CloneModule(*originalModule);
    passManager.run(*moduleClone);
  }
}

// Register the benchmark
BENCHMARK(llvmDeadCodeElimination)->Complexity()->Unit(benchmark::kMillisecond);

int main(int argc, char **argv) {
  // Initialize passes
  PassRegistry &passRegistry = *PassRegistry::getPassRegistry();
  initializeCore(passRegistry);
  initializeScalarOpts(passRegistry);
  initializeAnalysis(passRegistry);
  initializeTransformUtils(passRegistry);

  cl::ParseCommandLineOptions(argc, argv, "LLVM System Compiler\n");

  // Create context and parse the input bitcode file
  globalCtx = std::make_unique<LLVMContext>();
  SMDiagnostic diag;

  originalModule = parseIRFile(inputFileName, diag, *globalCtx);
  if (!originalModule) {
    diag.print(argv[0], errs());
    return 1;
  }

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  benchmark::Shutdown();

  // Explicitly clean up in correct order
  originalModule.reset();
  globalCtx.reset();

  return 0;
}
