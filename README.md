# MW Performance Benchmarking Suite

A high-performance benchmarking suite for LLVM Value Numbering operations, focusing on VNTable congruence testing and performance analysis.

## Overview

This project provides comprehensive performance benchmarks for LLVM-based value numbering operations, specifically targeting VNTable implementations. It includes both real-world LLVM IR tests and synthetic stress tests designed to evaluate the scalability and efficiency of different congruence algorithms.

### Key Features

- **VNTable Congruence Benchmarks**: Multiple implementation variants (naive, uniquify, parallel, fast lookup)
- **Synthetic IR Generation**: Stress tests with thousands of redundant expressions
- **Real LLVM IR Support**: Command-line specification of IR files for benchmarking
- **Parallel Performance Testing**: TBB-based parallel implementations
- **Comprehensive Test Suite**: GoogleTest-based unit tests with performance validation

## Prerequisites

### System Requirements
- **Operating System**: macOS (primary), Linux (supported)
- **C++ Compiler**: Clang++ with C++17 support
- **CMake**: Version 3.20 or higher

### Dependencies

#### LLVM
```bash
# Install LLVM via Homebrew (macOS)
brew install llvm

# Or install from source
# Ensure LLVM_DIR environment variable points to your LLVM installation
export LLVM_DIR=/usr/local/opt/llvm
```

#### Intel TBB (Threading Building Blocks)
```bash
# macOS
brew install tbb

# Linux (Ubuntu/Debian)
sudo apt-get install libtbb-dev
```

#### Boost Libraries
```bash
# macOS
brew install boost

# Linux (Ubuntu/Debian)
sudo apt-get install libboost-all-dev
```

#### Google Benchmark
```bash
# macOS
brew install google-benchmark

# Or build from source
git clone https://github.com/google/benchmark.git
cd benchmark
cmake -E make_directory "build"
cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../
cmake --build "build" --config Release
```

#### GoogleTest
```bash
# macOS
brew install googletest

# Or build from source (automatically handled by CMake if not found)
```

## Project Structure

```
mw_perf/
├── include/                    # Header files
│   ├── LLVMValueNumbering.hpp # Main value numbering interface
│   └── VNTable.hpp            # VNTable implementation
├── src/                       # Source implementations
├── tests/                     # Test suite
│   ├── llvm/                  # LLVM-specific tests
│   │   ├── tVNTableCongruenceSyntheticIR.cpp  # Synthetic IR benchmark
│   │   └── synthetic_heavy_gvn.ll             # Synthetic LLVM IR test data
│   └── CMakeLists.txt         # Test build configuration
├── benchmarks/                # Performance benchmarks
├── CMakeLists.txt            # Main build configuration
└── README.md                 # This file
```

## Building the Project

### 1. Clone and Setup
```bash
git clone <repository-url>
cd mw_perf
```

### 2. Configure Environment
```bash
# Set LLVM directory (adjust path as needed)
export LLVM_DIR=/usr/local/opt/llvm

# Verify LLVM installation
llvm-config --version
```

### 3. Build with CMake
```bash
# Create build directory
mkdir build && cd build

# Configure build
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build all targets
make -j$(nproc)
```

### 4. Alternative Debug Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

## Running Tests and Benchmarks

### Unit Tests
```bash
# Run all GoogleTest unit tests
cd build
make test

# Or run specific test executable
./tests/test_vntable
```

### Synthetic IR Benchmark
The synthetic IR benchmark tests VNTable performance on artificially generated LLVM IR with thousands of redundant expressions:

```bash
# Run synthetic benchmark with default IR file
cd build
./tests/tVNTableCongruenceSyntheticIR

# Run with custom IR file
./tests/tVNTableCongruenceSyntheticIR /path/to/your/ir/file.ll

# Run with the included synthetic test file
./tests/tVNTableCongruenceSyntheticIR ../tests/llvm/synthetic_heavy_gvn.ll
```

### Benchmark Variants
The synthetic benchmark includes multiple congruence implementations:
- **Naive**: Basic implementation for baseline comparison
- **Uniquify Naive**: Optimized naive with deduplication
- **Parallel Results Only**: Parallel processing of results
- **Fast**: Optimized sequential implementation
- **Parallel Lookup**: Parallel lookup operations
- **Redundant Focus**: Specialized for redundant expression handling

### Performance Analysis
```bash
# Run with detailed timing output
./tests/tVNTableCongruenceSyntheticIR --benchmark_time_unit=ms

# Export results to JSON
./tests/tVNTableCongruenceSyntheticIR --benchmark_format=json --benchmark_out=results.json

# Filter specific benchmarks
./tests/tVNTableCongruenceSyntheticIR --benchmark_filter="Fast.*"
```

## Synthetic IR Test Data

The `tests/llvm/synthetic_heavy_gvn.ll` file contains:
- **1000+ redundant arithmetic expressions**
- **Complex dependency chains**
- **Multiple basic blocks** for control flow testing
- **Various LLVM instruction types** (add, mul, load, store, phi nodes)

### Expanding Synthetic Tests
To create larger stress tests, modify the synthetic IR generation or create new `.ll` files:

```bash
# Example: Generate larger synthetic IR
# (You can extend the synthetic_heavy_gvn.ll file with more expressions)
```

## Troubleshooting

### Common Build Issues

#### "VNTable.hpp not found"
```bash
# Ensure include paths are correct in CMakeLists.txt
# Check that header files exist in include/ directory
ls -la include/
```

#### LLVM Configuration Issues
```bash
# Verify LLVM installation
llvm-config --includedir
llvm-config --libdir

# Check CMake can find LLVM
cmake .. -DLLVM_DIR=$(llvm-config --cmakedir)
```

#### Missing TBB or Boost
```bash
# macOS: Reinstall via Homebrew
brew reinstall tbb boost

# Linux: Install development packages
sudo apt-get install libtbb-dev libboost-all-dev
```

### Runtime Issues

#### Segmentation Faults
- **Cause**: Usually related to LLVM IR parsing or invalid memory access
- **Solution**: Run with debug build and gdb/lldb for stack traces
- **Verification**: Ensure IR files are valid LLVM IR

```bash
# Verify IR file validity
llvm-as < your_file.ll > /dev/null && echo "Valid IR" || echo "Invalid IR"

# Debug with lldb (macOS) or gdb (Linux)
lldb ./tests/tVNTableCongruenceSyntheticIR
(lldb) run ../tests/llvm/synthetic_heavy_gvn.ll
```

#### Performance Issues
- **Large IR files**: Consider sampling or chunking large IR files
- **Memory usage**: Monitor with `top` or `htop` during benchmark runs
- **Parallel overhead**: Compare parallel vs sequential implementations

## Development Guidelines

### Adding New Benchmarks
1. Create new `.cpp` file in `tests/llvm/` or `benchmarks/`
2. Include necessary headers with correct relative paths
3. Follow existing benchmark patterns using Google Benchmark macros
4. Update `tests/CMakeLists.txt` to include new executable

### Header Include Patterns
```cpp
// Correct include paths for project headers
#include "../../include/VNTable.hpp"
#include "../../include/LLVMValueNumbering.hpp"

// System and third-party includes
#include <llvm/IR/Module.h>
#include <benchmark/benchmark.h>
```

### Testing New IR Files
```cpp
// Template for command-line IR file support
int main(int argc, char** argv) {
    std::string ir_file = (argc > 1) ? argv[1] : "default_file.ll";
    // ... benchmark implementation
}
```

## Performance Metrics

Expected benchmark outputs include:
- **Time per operation**: Microseconds for VNTable operations
- **Memory usage**: Peak memory consumption during benchmarks
- **Scalability**: Performance vs. IR file size/complexity
- **Parallel efficiency**: Speedup ratios for parallel implementations

## Documentation and Analysis Results

The `doc/` directory contains comprehensive analysis results and documentation for the benchmarking outcomes:

```
doc/
├── LLVM_VALUE_NUMBERING_USAGE.md      # LLVM value numbering usage guide
├── VNTableCongruence_Performance_Analysis.md  # Detailed performance analysis
├── VNTableCongruence_Raw_Output.txt    # Complete benchmark output logs
└── VNTableCongruence_Results_Summary.csv  # Summary results in CSV format
```

### Key Documentation Files

#### `LLVM_VALUE_NUMBERING_USAGE.md`
Comprehensive guide covering:
- LLVM value numbering concepts and implementation
- VNTable architecture and design patterns
- Best practices for performance optimization
- Integration guidelines for LLVM-based projects

#### `VNTableCongruence_Performance_Analysis.md`
In-depth performance analysis including:
- Benchmark methodology and test scenarios
- Comparative analysis of different VNTable implementations
- Performance scaling characteristics
- Memory usage patterns and optimization opportunities
- Recommendations for production use

#### `VNTableCongruence_Raw_Output.txt`
Complete raw benchmark output containing:
- Detailed timing measurements for all test variants
- System configuration and environment details
- Full benchmark execution logs
- Error handling and edge case results

#### `VNTableCongruence_Results_Summary.csv`
Structured results data for analysis:
- Performance metrics in tabular format
- Easy import into spreadsheet applications
- Statistical summaries and comparisons
- Baseline performance benchmarks

### Using the Analysis Results

```bash
# View performance analysis
cat doc/VNTableCongruence_Performance_Analysis.md

# Import CSV results for further analysis
# Use with Excel, Google Sheets, or data analysis tools
open doc/VNTableCongruence_Results_Summary.csv

# Review complete benchmark logs
less doc/VNTableCongruence_Raw_Output.txt

# Access LLVM implementation guide
cat doc/LLVM_VALUE_NUMBERING_USAGE.md
```

### Interpreting Results

The analysis documents provide:
- **Performance baselines** for different VNTable implementations
- **Scalability insights** for various IR complexity levels
- **Memory efficiency** comparisons across test scenarios
- **Parallel vs sequential** performance trade-offs
- **Production deployment** recommendations

Refer to these documents when:
- Choosing optimal VNTable implementation for your use case
- Understanding performance characteristics of different approaches
- Troubleshooting performance issues in production
- Planning performance optimization strategies

## Contributing

1. Fork the repository
2. Create feature branch (`git checkout -b feature/new-benchmark`)
3. Add tests for new functionality
4. Ensure all tests pass (`make test`)
5. Submit pull request with performance impact analysis

## License

[Specify your license here]

## Contact

[Add contact information or links to project maintainers]
