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
- `cache_locality/` - Sequential vs strided memory access comparison
- `branch_prediction/` - Predictable vs unpredictable branch patterns comparison
- `ilp_no_data_dependencies/` - ILP through loop unrolling with independent operations
- `ilp_data_dependencies/` - ILP impact of data dependencies between loop iterations
- `containers/vector/` - Multi-binary project comparing 5 vector implementations (std, boost variants, abseil)
- `skeleton/` - Template for creating new projects

**Infrastructure:**
- `3rdparty/` - Third-party dependencies (organized structure)
  - `src/` - Source code (google/benchmark, boost, abseil-cpp)
  - `.build/` - Pre-built libraries (benchmark, abseil)
- `cmake/` - Common CMake configuration
- `.build/` - Centralized build directory (all projects and configurations)
- `.benchmarks/` - Benchmark results organized by project
- `.disassembly/` - Disassembly outputs organized by project

**Scripts:**
- `benchmarks.sh` - Run benchmarks for a project across all compilers/optimization levels
- `disassembly.sh` - Generate disassembly for a project
- `run_all.sh` - Run benchmarks and disassembly for all projects
- `build_common.sh` - Shared build helper functions

**Isolated Builds (Docker-based):**
- `isolated_builds/` - Build with multiple compiler versions using Docker
  - `build.sh` - Compile with GCC 7-13 and Clang 6-18 for optimization evolution analysis
  - `benchmarks.sh` - Run Docker-built binaries natively on host
  - `disassembly.sh` - Generate disassembly from Docker builds
  - `run_all.sh` - Run build + benchmarks + disassembly for all projects
  - `README.md` - Detailed workflow documentation

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

### 5. cache_locality
Compares sequential vs strided memory access patterns to demonstrate cache locality effects. Uses a 16MB array with sequential iteration vs 1024-element stride (4KB jumps) to show impact of cache misses and hardware prefetching.

### 6. branch_prediction
Compares predictable vs unpredictable branch patterns to demonstrate branch predictor impact. Uses array of random numbers [1, 100] with conditional swaps based on sum thresholds: threshold 195 (highly predictable, ~97% same outcome) vs threshold 100 (~50% unpredictable, causes branch mispredictions).

### 7. ilp_no_data_dependencies
Demonstrates Instruction-Level Parallelism (ILP) through loop unrolling. Compares sequential swaps (one per iteration) vs unrolled swaps (four independent swaps per iteration). The unrolled version allows CPU to execute multiple swap operations in parallel using superscalar execution, reducing loop overhead.

### 8. ilp_data_dependencies
Demonstrates impact of data dependencies on ILP. Compares independent iterations (each iteration uses current pair average) vs dependent iterations (uses previous pair average via prev_avg variable). The dependency chain in the dependent version prevents CPU from parallelizing iterations, showing ~2-3x performance difference.

### 9. containers/vector
**Multi-binary benchmark project** comparing 5 vector container implementations:
- `std::vector` - Standard library vector (heap-based)
- `boost::container::vector` - Boost vector (heap-based, optimized growth)
- `boost::container::small_vector` - Small buffer optimization (8 elements inline, then heap)
- `boost::container::static_vector` - Fully stack-based (fixed capacity)
- `absl::InlinedVector` - Google Abseil inline vector (8 elements inline)

**Structure** (multiple binaries instead of single main.cpp):
- `common.h` - Shared data structures (SmallElement, Point, LargeStruct)
- `bench_insert.cpp` - Insert benchmarks (Small: 8, Medium: 64, Large: 1024 elements)
- `bench_copy.cpp` - Copy benchmarks (Small/Medium/Large)
- `bench_iterate.cpp` - Iteration benchmarks (Small/Medium/Large)

**Hierarchical naming**: Benchmarks use names like `Insert/Small_int/StdVector` for automatic grouping by operation/size/element-type.

### 10. skeleton
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

### Third-Party Dependencies

**Organization** (`3rdparty/` directory):
```
3rdparty/
├── src/                    # Source code (cloned once, shared)
│   ├── benchmark/          # Google Benchmark
│   ├── boost/              # Boost (header-only: container, config, assert, etc.)
│   └── abseil-cpp/         # Google Abseil
└── .build/                 # Pre-built libraries (compiled once, reused)
    ├── benchmark/          # Google Benchmark build
    └── abseil/             # Abseil build (for hash containers)
```

**Key Benefits**:
- **No rebuilds**: Dependencies built once per system, reused across all projects
- **Shared source**: Single clone of each library, no duplication
- **Automatic setup**: CMake clones and builds missing dependencies on first run
- **Fast configuration**: Projects just link pre-built `.a` files

**Dependency Details**:
1. **Google Benchmark**: Always required, built in Release mode with NDEBUG
2. **Boost**: Header-only (container, config, assert, type_traits, core, move, intrusive)
3. **Abseil**: Builds `libabsl_raw_hash_set.a` and `libabsl_hashtablez_sampler.a` for hash containers

**Nested Project Support**: Path calculation works for both flat (`inlining/`) and nested (`containers/vector/`) projects.

### Multi-Binary Projects

Some projects (like `containers/vector`) use multiple benchmark binaries instead of single `main.cpp`:

**Pattern**:
```
containers/vector/
├── common.h              # Shared definitions
├── bench_insert.cpp      # Binary 1: Insert benchmarks
├── bench_copy.cpp        # Binary 2: Copy benchmarks  
├── bench_iterate.cpp     # Binary 3: Iterate benchmarks
└── bench_clearrefill.cpp # Binary 4: ClearRefill benchmarks
```

**CMakeLists.txt** creates multiple executables:
```cmake
add_executable(bench_insert bench_insert.cpp)
add_executable(bench_copy bench_copy.cpp)
# ... etc
```

**Benefits**:
- Logical separation by operation type
- Easier navigation and maintenance
- Can run subsets: `--benchmark_filter="Insert/Medium"`

**benchmarks.sh** automatically detects and runs all `bench_*` binaries in build directory.

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
========== clang -O1 ==========
<benchmark results>
...
```

### `generate_all_summaries.sh`

Generates `summary.md` files from `benchmark.log` for easy comparison.

**Features**:
- Extracts results for all optimization levels (O0, O1, O2, O3)
- Creates formatted comparison tables
- **Column order**: clang-O3, gcc-O3, clang-O2, gcc-O2, clang-O1, gcc-O1, clang-O0, gcc-O0
  - Highest optimization (O3) on the left for quick reference
  - Lower levels on the right for optional review
- **Decimal point alignment**: Numbers padded with zeros for visual alignment
- Auto-detects simple vs hierarchical benchmark names

**Output**: `.benchmarks/<project>/summary.md`

**Table format examples**:

Simple names (exception):
```markdown
| Benchmark               |     clang-O3 |       gcc-O3 |     clang-O2 |       gcc-O2 | ... |
|-------------------------|--------------|--------------|--------------|--------------|-----|
| BM_process_exception    | 194520176 ns | 171356889 ns | 191656405 ns | 171041887 ns | ... |
| BM_process_return_code  |   2225705 ns |   1995060 ns |   2095378 ns |   2042145 ns | ... |
```

Hierarchical names (containers/vector):
```markdown
## Copy/Small_int

| Container     | clang-O3 |   gcc-O3 | clang-O2 |   gcc-O2 | ... |
|---------------|----------|----------|----------|----------|-----|
| BoostVector   | 14.5  ns | 16.5  ns | 14.6  ns | 16.5 ns  | ... |
| StaticVector  |  1.72 ns |  1.32 ns |  1.73 ns |  1.33 ns | ... |
```

**Usage**:
```bash
./generate_all_summaries.sh              # All projects
python3 generate_summary.py <log_path>   # Single project
```

**Implementation**: Python script (`generate_summary.py`) parses `benchmark.log`, groups results, and generates markdown with proper formatting and alignment.

### `disassembly.sh <project>`

**Purpose**: Generate disassembly for analysis across all configurations

**Usage**:
```bash
./disassembly.sh <project_name>
# Example: ./disassembly.sh inlining
```

**Configuration**: Same as `benchmarks.sh`

**Output**: `.disassembly/<project>/<compiler>_<level>.dis`

**Features**:
- Uses `objdump -d -C` for disassembly with demangled names
- Strips memory addresses for easier comparison
- Extracts functions containing `prfct_` prefix (both regular and template functions)
- Automatically discovers relevant functions (no hardcoding needed)

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

### build_common.sh

**Purpose**: Shared build functions to eliminate code duplication between automation scripts

**Usage**: Sourced by `benchmarks.sh` and `disassembly.sh`:
```bash
source "${SCRIPT_DIR}/build_common.sh"
build_project "${PROJECT_DIR}" "${BUILD_DIR}" "${compiler}" "${opt_level}"
```

**Key Function**:
- `build_project <project_dir> <build_dir> <compiler> <opt_level>` - Executes CMake configure and build

**Why It Exists**:
- Eliminates duplicate build logic across scripts
- Ensures consistent build process
- Single point of maintenance for build commands

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
