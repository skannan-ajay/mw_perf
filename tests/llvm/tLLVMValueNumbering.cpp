#include "../../include/LLVMValueNumbering.hpp"
#include <benchmark/benchmark.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/SourceMgr.h>
#include <iostream>
#include <memory>
#include <map>

using namespace llvm;

// Create a simple test function with redundant expressions
std::unique_ptr<Module> createTestModule(LLVMContext& context) {
    auto module = std::make_unique<Module>("test", context);
    
    // Create function: int test(int a, int b)
    FunctionType* funcType = FunctionType::get(
        Type::getInt32Ty(context),
        {Type::getInt32Ty(context), Type::getInt32Ty(context)},
        false
    );
    
    Function* func = Function::Create(
        funcType, 
        Function::ExternalLinkage, 
        "test", 
        module.get()
    );
    
    // Create basic block
    BasicBlock* bb = BasicBlock::Create(context, "entry", func);
    IRBuilder<> builder(bb);
    
    // Get function arguments
    auto args = func->arg_begin();
    Value* a = &*args++;
    Value* b = &*args;
    a->setName("a");
    b->setName("b");
    
    // Create some expressions with redundancy
    // x = a + b
    Value* x = builder.CreateAdd(a, b, "x");
    
    // y = a + b  (same as x - should get same value number)
    Value* y = builder.CreateAdd(a, b, "y");
    (void)y; // Suppress unused variable warning
    
    // z = b + a  (commutative, should get same value number as x and y)
    Value* z = builder.CreateAdd(b, a, "z");
    (void)z; // Suppress unused variable warning
    
    // w = a * b  (different operation)
    Value* w = builder.CreateMul(a, b, "w");
    
    // result = x + w
    Value* result = builder.CreateAdd(x, w, "result");
    
    builder.CreateRet(result);
    
    return module;
}

// Global variables for module loading
std::unique_ptr<LLVMContext> globalContext;
std::unique_ptr<Module> globalModule;

// Benchmark value numbering performance
static void BM_ValueNumbering(benchmark::State& state) {
    if (!globalModule) {
        state.SkipWithError("Module not loaded");
        return;
    }
    
    LLVMValueNumbering vn;
    
    // Collect all instructions
    std::vector<Value*> instructions;
    for (auto& func : *globalModule) {
        for (auto& bb : func) {
            for (auto& inst : bb) {
                instructions.push_back(&inst);
            }
        }
    }
    
    for (auto _ : state) {
        vn.clear();
        
        // Compute value numbers for all instructions
        for (auto* inst : instructions) {
            auto valueNumber = vn.getValueNumber(inst);
            benchmark::DoNotOptimize(valueNumber);
        }
    }
    
    state.SetItemsProcessed(state.iterations() * instructions.size());
}

static cl::opt<std::string> InputFile(cl::Positional, 
    cl::desc("<input IR file (.ll or .bc)> - if not specified, uses built-in test module"), 
    cl::init("-"));

// Load module from the input file
std::unique_ptr<Module> loadModuleFromFile(LLVMContext &context) {
    SMDiagnostic Err;
    auto module = parseIRFile(InputFile, Err, context);
    if (!module) {
        Err.print("tLLVMValueNumbering", errs());
        return nullptr;
    }
    return module;
}

// Test function to demonstrate value numbering
void demonstrateValueNumbering(Module *module) {
    LLVMValueNumbering vn;
    
    std::cout << "=== LLVM Value Numbering Demonstration ===" << std::endl;
    std::cout << "Module: " << module->getName().str() << std::endl;
    
    // Statistics tracking
    std::map<LLVMValueNumbering::ValueNumber, std::vector<llvm::Value*>> vnGroups;
    int totalInstructions = 0;
    int totalFunctions = 0;
    
    // Process all functions in the module
    for (auto& func : *module) {
        if (func.isDeclaration()) continue; // Skip function declarations
        totalFunctions++;
        
        std::cout << "\n--- Function: " << func.getName().str() << " ---" << std::endl;
        
        // Process all instructions and collect value numbers
        for (auto& bb : func) {
            std::cout << "  Basic Block: " << bb.getName().str() << std::endl;
            for (auto& inst : bb) {
                auto vn_value = vn.getValueNumber(&inst);
                totalInstructions++;
                
                // Group instructions by value number
                vnGroups[vn_value].push_back(&inst);
                
                std::cout << "    Instruction: ";
                inst.print(outs());
                std::cout << " => Value Number: " << vn_value << std::endl;
            }
        }
        
        // Only analyze the first function for detailed output
        if (totalFunctions == 1) {
            // Find congruent expressions in this function
            std::cout << "\n    === Congruence Analysis for " << func.getName().str() << " ===" << std::endl;
            
            Value* firstAdd = nullptr;
            for (auto& bb : func) {
                for (auto& inst : bb) {
                    if (inst.getOpcode() == Instruction::Add && inst.getNumOperands() == 2) {
                        if (!firstAdd) {
                            firstAdd = &inst;
                            auto congruent = LLVMValueNumberingUtils::findCongruentExpressions(vn, firstAdd);
                            
                            std::cout << "    Expressions congruent to: ";
                            firstAdd->print(outs());
                            std::cout << std::endl;
                            
                            for (auto* expr : congruent) {
                                std::cout << "      - ";
                                expr->print(outs());
                                std::cout << std::endl;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    
    // Generate GVN Statistics
    std::cout << "\n=== GVN Analysis Summary ===" << std::endl;
    std::cout << "Total Functions: " << totalFunctions << std::endl;
    std::cout << "Total Instructions: " << totalInstructions << std::endl;
    std::cout << "Unique Value Numbers: " << vnGroups.size() << std::endl;
    
    // Count GVN candidates (value numbers with multiple instructions)
    int gvnCandidates = 0;
    int totalRedundantInstructions = 0;
    
    for (const auto& pair : vnGroups) {
        if (pair.second.size() > 1) {
            gvnCandidates++;
            totalRedundantInstructions += (pair.second.size() - 1); // All but one are redundant
        }
    }
    
    std::cout << "GVN Candidate Groups: " << gvnCandidates << std::endl;
    std::cout << "Total Redundant Instructions: " << totalRedundantInstructions << std::endl;
    
    if (gvnCandidates > 0) {
        std::cout << "\n=== GVN Candidate Details ===" << std::endl;
        int groupNum = 1;
        for (const auto& pair : vnGroups) {
            if (pair.second.size() > 1) {
                std::cout << "Group " << groupNum++ << " (VN: " << pair.first << ", Count: " << pair.second.size() << "):" << std::endl;
                for (auto* inst : pair.second) {
                    std::cout << "  ";
                    inst->print(outs());
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        }
    } else {
        std::cout << "No GVN candidates found - all instructions have unique value numbers." << std::endl;
    }
}

BENCHMARK(BM_ValueNumbering)
    ->Unit(benchmark::kMicrosecond)
    ->Iterations(10000);

int main(int argc, char** argv) {
    // Parse command line arguments first
    cl::ParseCommandLineOptions(argc, argv, "LLVM Value Numbering Analysis\n");
    
    // Initialize global context and load module
    globalContext = std::make_unique<LLVMContext>();
    
    // If no input file specified, create a test module
    if (InputFile == "-") {
        std::cout << "No input file specified, creating test module...\n";
        globalModule = createTestModule(*globalContext);
    } else {
        globalModule = loadModuleFromFile(*globalContext);
        if (!globalModule) {
            return 1;
        }
    }
    
    // Run demonstration first
    demonstrateValueNumbering(globalModule.get());
    
    // Then run benchmarks
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    
    return 0;
}
