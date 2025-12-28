# Perfection Framework

A framework for benchmarking and analyzing compiler optimizations through C++ micro-projects.

## Quick Start

```bash
# Run benchmarks for a project
./benchmarks.sh exception

# Generate disassembly
./disassembly.sh exception

# Process all projects
./run_all.sh

# View results
cat .benchmarks/exception/benchmark.log
cat .disassembly/exception/clang_O3.dis
```

## Project Structure

```
perfection/
├── .build/              # Centralized builds (all projects & configs)
│   └── <project>/
│       ├── clang_O0/
│       ├── clang_O1/
│       ├── gcc_O3/
│       └── ...
├── .benchmarks/         # Benchmark results by project
│   └── <project>/
│       └── benchmark.log
├── .disassembly/        # Disassembly by project
│   └── <project>/
│       ├── clang_O3.dis
│       └── gcc_O3.dis
├── cmake/               # Common CMake configuration
├── benchmark/           # Google Benchmark (auto-installed)
├── inlining/            # Inlining comparison project
├── virtual/             # Virtual dispatch comparison
├── noexcept/            # noexcept impact comparison
├── exception/           # Exception vs return codes
└── skeleton/            # Template for new projects
```

## Projects

### inlining
Compares `FORCE_INLINE` vs `NOINLINE` function attributes.

### virtual
Compares virtual vs non-virtual function calls.

### noexcept
Tests performance impact of `noexcept` specifier.

### exception
Compares exception handling vs return codes for error handling.

### skeleton
Template for creating new comparison projects.

## Build System

All projects use a shared CMake configuration (`cmake/PerfectionCommon.cmake`).

**Each project's CMakeLists.txt** (5-10 lines):
```cmake
cmake_minimum_required(VERSION 3.10)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/PerfectionCommon.cmake)
project(my_project VERSION 1.0)
perfection_setup_project(my_project)
```

**Build options**:
- `COMPILER_CHOICE`: `gcc` (default) or `clang`
- `OPTIMIZATION_LEVEL`: `O0`, `O1`, `O2` (default), `O3`, `Os`, `Ofast`

## Key Features

### Centralized Build Directory
- **No redundant rebuilds**: Each configuration stored separately
- **Fast re-runs**: Scripts only rebuild what changed
- **Easy management**: All builds in `.build/<project>/<compiler>_<opt>/`

### Organized Results
- Benchmarks: `.benchmarks/<project>/benchmark.log`
- Disassembly: `.disassembly/<project>/<compiler>_<opt>.dis`

### Automation Scripts
- `benchmarks.sh <project>` - Run all configurations (2 compilers × 4 opt levels)
- `disassembly.sh <project>` - Generate disassembly for all configurations
- `run_all.sh` - Process all projects at once

## Adding a New Project

1. **Copy skeleton**:
   ```bash
   cp -r skeleton my_project
   ```

2. **Update CMakeLists.txt**:
   - Change `project(skeleton ...)` to `project(my_project ...)`
   - Change `perfection_setup_project(skeleton)` to `perfection_setup_project(my_project)`

3. **Update main.cpp**: Add your benchmark code

4. **Test**:
   ```bash
   ./benchmarks.sh my_project
   ./disassembly.sh my_project
   ```

## Example Workflow

```bash
# Benchmark exception handling vs return codes
./benchmarks.sh exception

# View results
cat .benchmarks/exception/benchmark.log | grep "BM_"

# Compare Clang vs GCC at O3
diff .disassembly/exception/clang_O3.dis .disassembly/exception/gcc_O3.dis

# Run all projects
./run_all.sh
```

## Documentation

See [context.md](context.md) for detailed documentation.

## Requirements

- CMake 3.10+
- GCC and/or Clang
- Git (for auto-installing Google Benchmark)
