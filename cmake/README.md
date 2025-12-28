# CMake Common Configuration

This directory contains shared CMake configuration files used by all projects in the Perfection framework.

## PerfectionCommon.cmake

Central configuration file that provides:

- **Compiler Selection**: Supports GCC and Clang with automatic detection
- **Optimization Levels**: O0, O1, O2, O3, Os, Ofast
- **Google Benchmark Integration**: Automatic download, build, and linking
- **Project Setup Function**: `perfection_setup_project(project_name)`

## Usage in Projects

Each project's `CMakeLists.txt` should follow this pattern:

```cmake
cmake_minimum_required(VERSION 3.10)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/PerfectionCommon.cmake)
project(my_project VERSION 1.0)
perfection_setup_project(my_project)
```

This minimal structure provides:
- Compiler selection via `-DCOMPILER_CHOICE=gcc|clang`
- Optimization control via `-DOPTIMIZATION_LEVEL=O0|O1|O2|O3|Os|Ofast`
- Automatic Google Benchmark setup
- Executable creation and linking

## Benefits

- **DRY Principle**: No code duplication across projects
- **Consistency**: All projects use identical build configuration
- **Maintainability**: Single point of change for build system updates
- **Simplicity**: Project CMakeLists.txt files are 5-10 lines instead of 100+
