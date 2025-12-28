# Perfection Project - Context Documentation

## Project Overview

**Purpose**: A framework for benchmarking and analyzing compiler optimizations through multiple C++ micro-projects. Each project compares different optimization strategies (e.g., inlining vs non-inlining) across compilers (GCC/Clang) and optimization levels (O0-O3).

**Location**: `/home/lipkin/dev/o/perfection/`

**Key Innovation**: Multi-project infrastructure with centralized automation scripts for benchmarking and disassembly analysis.

---

## Project Structure

**Benchmark Projects:**
- `inlining/` - Inlining comparison (FORCE_INLINE vs NOINLINE)
- `virtual/` - Virtual function dispatch comparison
- `noexcept/` - noexcept specifier impact comparison
- `exception/` - Exception handling vs return codes comparison
- `skeleton/` - Template for creating new projects

**Infrastructure:**
- `benchmark/` - Google Benchmark library (auto-installed)
- `cmake/` - Common CMake configuration
- `.build/` - Centralized build directory (all projects and configurations)
- `.benchmarks/` - Benchmark results organized by project
- `.disassembly/` - Disassembly outputs organized by project

**Scripts:**
- `benchmarks.sh` - Run benchmarks for a project across all compilers/optimization levels
- `disassembly.sh` - Generate disassembly for a project
- `run_all.sh` - Process all projects at once

**Documentation:**
- `context.md` - This file, project documentation
- `candidates.md` - Ideas for future optimization tests
- `.gitignore` - Excludes build/output directories

---

## Current Projects

### 1. inlining
Compares performance impact of function inlining using `FORCE_INLINE` vs `NOINLINE` attributes. Benchmarks reverse a 1MB array using inlined and non-inlined swap functions.

### 2. virtual
Compares virtual function calls vs non-virtual function calls. Uses class hierarchy with base class pointer to force vtable lookup in virtual version.

### 3. noexcept
Compares performance impact of `noexcept` specifier on virtual functions. Tests whether declaring functions as `noexcept` affects runtime performance.

### 4. exception
Compares exception handling vs return code error handling. Benchmarks swap operations that fail based on parity checks, using exceptions vs return codes.

### 5. skeleton
Empty template project for creating new optimization comparison tests. Contains placeholder functions and benchmarks ready to be customized.

---

## CMake Build System

### Common Configuration

All projects use a shared CMake configuration located in `cmake/PerfectionCommon.cmake`. This eliminates code duplication and ensures consistent build settings across all projects.

**Key Benefits**:
- Centralized compiler selection logic
- Shared Google Benchmark setup
- Consistent optimization level handling
- Easy maintenance and updates

### Project Structure

Each project's `CMakeLists.txt` is minimal (5-10 lines):
```cmake
cmake_minimum_required(VERSION 3.10)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/PerfectionCommon.cmake)
project(my_project VERSION 1.0)
perfection_setup_project(my_project)
```

### Compiler Selection

Each project's `CMakeLists.txt` supports:

**Option**: `COMPILER_CHOICE`
- Values: `gcc` (default), `clang`
- Usage: `-DCOMPILER_CHOICE=clang`

**Option**: `OPTIMIZATION_LEVEL`
- Values: `O0`, `O1`, `O2` (default), `O3`, `Os`, `Ofast`
- Usage: `-DOPTIMIZATION_LEVEL=O3`

### Google Benchmark Integration

- Automatically clones from GitHub if not found in `../benchmark`
- Builds in `../benchmark/build`
- Shared across all projects

### Example Build

```bash
cd inlining
cmake -S . -B ../.build/inlining/gcc_O3 -DCOMPILER_CHOICE=gcc -DOPTIMIZATION_LEVEL=O3
cmake --build ../.build/inlining/gcc_O3
../.build/inlining/gcc_O3/inlining
```

**Note**: Manual builds are stored in `.build/<project>/<compiler>_<opt_level>/` to keep all builds organized and persistent.

---

## Automation Scripts

### Build Structure

All builds are centralized in `.build/<project>/<compiler>_<opt_level>/` structure:

**Benefits**:
- **No Rebuilds**: Different configurations don't overwrite each other
- **Fast Iteration**: Re-running scripts only rebuilds changed code
- **Easy Comparison**: All binaries available side-by-side
- **Organized Output**: Results grouped by project in `.benchmarks/` and `.disassembly/`

**Example Structure**:
```
.build/
  exception/
    clang_O0/
    clang_O1/
    clang_O2/
    clang_O3/
    gcc_O0/
    ...
  inlining/
    clang_O0/
    ...
.benchmarks/
  exception/
    benchmark.log
  inlining/
    benchmark.log
.disassembly/
  exception/
    clang_O3.dis
    gcc_O3.dis
    ...
  inlining/
    clang_O3.dis
    ...
```

### benchmarks.sh

**Purpose**: Run benchmarks for a project across all compiler/optimization combinations

**Usage**: 
```bash
./benchmarks.sh <project_name>
# Example: ./benchmarks.sh inlining
```

**Configuration**:
- Compilers: `clang`, `gcc`
- Optimization levels: `O0`, `O1`, `O2`, `O3`
- Total runs: 8 (2 compilers × 4 levels)

**Output**: `.benchmarks/<project>/benchmark.log`

**Format**:
```
========== clang -O0 ==========
<benchmark results>

========== clang -O1 ==========
<benchmark results>
...
```

### disassembly.sh

**Purpose**: Generate disassembly for analysis across all configurations

**Usage**:
```bash
./disassembly.sh <project_name>
# Example: ./disassembly.sh inlining
```

**Configuration**: Same as `benchmarks.sh`

**Output**: `.disassembly/<project>/<compiler>_<level>.dis`

**Features**:
- Uses `objdump -d -C` for disassembly
- Strips memory addresses for easier comparison
- Extracts specific functions (e.g., `process_random_data_inlined`)

**Example Output File**: `.disassembly/inlining/clang_O3.dis`

### run_all.sh

**Purpose**: Process all projects in one command

**Usage**:
```bash
./run_all.sh
```

**Projects List**: Edit `PROJECTS=("inlining" "virtual" "noexcept" "exception")` to add more

**Workflow**:
1. For each project:
   - Run `benchmarks.sh <project>`
   - Run `disassembly.sh <project>`

---

## Adding a New Project

### Step-by-Step

1. **Copy skeleton**:
   ```bash
   cp -r skeleton my_new_project
   cd my_new_project
   ```

2. **Update CMakeLists.txt**:
   - Change `project(skeleton ...)` → `project(my_new_project ...)`
   - Change `perfection_setup_project(skeleton)` → `perfection_setup_project(my_new_project)`

3. **Update main.cpp**:
   - Rename functions: `skeleton()` → `my_function()`
   - Rename benchmarks: `BM_skeleton` → `BM_my_function`
   - Add your implementation

4. **Add to run_all.sh**:
   ```bash
   PROJECTS=("inlining" "exception" "my_new_project")
   ```

5. **Test**:
   ```bash
   ./benchmarks.sh my_new_project
   ./disassembly.sh my_new_project
   ```

---

## Workflow Examples

### Compare Inlining Impact with GCC O3

```bash
./benchmarks.sh inlining
grep "gcc -O3" .benchmarks/inlining/benchmark.log -A 10

./disassembly.sh inlining
cat .disassembly/inlining/gcc_O3.dis
```

### Compare Clang vs GCC at O2

```bash
# Run benchmarks
./benchmarks.sh inlining

# Compare disassembly
diff .disassembly/inlining/clang_O2.dis .disassembly/inlining/gcc_O2.dis
```

### Full Analysis of All Projects

```bash
./run_all.sh

# View all benchmark results
ls .benchmarks/
cat .benchmarks/inlining/benchmark.log
cat .benchmarks/exception/benchmark.log

# View all disassembly
ls .disassembly/
```

---

## Important Files

### Configuration
- [`.gitignore`](file:///home/lipkin/dev/o/perfection/.gitignore) - excludes build/output directories

### Automation
- [`benchmarks.sh`](file:///home/lipkin/dev/o/perfection/benchmarks.sh) - benchmark runner
- [`disassembly.sh`](file:///home/lipkin/dev/o/perfection/disassembly.sh) - disassembly generator
- [`run_all.sh`](file:///home/lipkin/dev/o/perfection/run_all.sh) - orchestration

### Projects
- [`inlining/main.cpp`](file:///home/lipkin/dev/o/perfection/inlining/main.cpp) - inlining comparison
- [`exception/main.cpp`](file:///home/lipkin/dev/o/perfection/exception/main.cpp) - exception handling comparison
- [`skeleton/main.cpp`](file:///home/lipkin/dev/o/perfection/skeleton/main.cpp) - project template

---

## Common Commands

```bash
# Build single project manually (organized in centralized .build/)
cd inlining
cmake -S . -B ../.build/inlining/gcc_O2 -DCOMPILER_CHOICE=gcc -DOPTIMIZATION_LEVEL=O2
cmake --build ../.build/inlining/gcc_O2
../.build/inlining/gcc_O2/inlining

# Run benchmarks for one project (fast, uses cached builds)
./benchmarks.sh inlining

# Generate disassembly for one project
./disassembly.sh inlining

# Process all projects
./run_all.sh

# View benchmark results (organized by project)
cat .benchmarks/inlining/benchmark.log
cat .benchmarks/exception/benchmark.log

# View disassembly (organized by project)
cat .disassembly/inlining/gcc_O3.dis

# Compare disassemblies
diff .disassembly/inlining/clang_O3.dis .disassembly/inlining/gcc_O3.dis

# Clean specific build configuration
rm -rf .build/inlining/gcc_O3

# Clean all builds for a project
rm -rf .build/inlining/

# Clean everything
rm -rf .build/ .benchmarks/ .disassembly/
```

---

## Design Decisions

### Why Multi-Project Structure?

Allows systematic comparison of different optimization strategies without code duplication. Each project focuses on one specific optimization technique.

### Why Centralized Build Directory?

**Benefits**:
- **No Redundant Rebuilds**: Different compiler/optimization configurations coexist
- **Fast Iteration**: Re-running scripts only builds what changed
- **Easy Management**: All builds in one place (`.build/`)
- **Results Organization**: Outputs grouped by project in `.benchmarks/` and `.disassembly/`

**Structure**: `.build/<project>/<compiler>_<optlevel>/` keeps everything organized and accessible.

### Why Centralized Scripts?

Reduces maintenance burden. Single script update applies to all projects. Ensures consistent benchmarking methodology.

### Why Strip Addresses from Disassembly?

Memory addresses change between builds, making `diff` comparisons noisy. Stripping addresses focuses on actual instruction differences.

### Why Both Benchmarks and Disassembly?

- **Benchmarks**: Quantify performance impact
- **Disassembly**: Understand *why* performance differs (verify inlining, analyze instruction count)

---

## Future Expansion Ideas

1. **New Projects**:
   - Loop unrolling comparison
   - Branch prediction optimization
   - Cache-friendly data structures
   - SIMD vectorization

2. **Additional Analysis**:
   - `perf` profiling integration
   - Cache miss analysis
   - Instruction count comparison

---

## Troubleshooting

### Build Failures

**Issue**: Google Benchmark not found
**Solution**: Delete `benchmark/` and let CMake re-clone

**Issue**: Compiler not found
**Solution**: Install gcc/clang: `sudo apt install gcc clang`

### Script Errors

**Issue**: `./benchmarks.sh: Permission denied`
**Solution**: `chmod +x benchmarks.sh disassembly.sh run_all.sh`

**Issue**: Project directory not found
**Solution**: Ensure project exists in perfection root

### Empty Disassembly

**Issue**: Function not found in disassembly
**Solution**: Check function names in `disassembly.sh` match actual function names
