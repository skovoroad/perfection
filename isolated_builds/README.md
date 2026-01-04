# Isolated Builds Infrastructure

This directory contains scripts for building and testing the perfection benchmarks using Docker containers with different compiler versions.

## Why Isolated Builds?

- **Multiple compiler versions** without installing them all on host
- **Reproducible builds** with fixed compiler versions
- **Isolation** - different compilers don't conflict
- **Native execution** - binaries run on host for accurate benchmarking

## Directory Structure

```
isolated_builds/
├── build.sh               # Build with compiler versions GCC 7-13, Clang 6-18
├── benchmarks.sh          # Run Docker-built binaries on host
├── disassembly.sh         # Generate disassembly from Docker builds
├── run_all.sh             # Run build + benchmarks + disassembly for all projects
├── .build/                # Docker build outputs (gitignored)
├── .benchmarks/           # Benchmark results (gitignored)
└── .disassembly/          # Disassembly outputs (gitignored)
```

## Workflow

### 1. Build with Docker (wide compiler range)

```bash
cd isolated_builds
./build.sh ilp_data_dependencies
```

This builds with compiler versions spanning 2017-2024 to observe optimization evolution:
- **GCC**: 7, 8, 9, 10, 11, 12, 13 (2017-2023)
- **Clang**: 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 (2018-2024)
- Each at O0, O1, O2, O3 optimization levels
- Output statically-linked binaries to `isolated_builds/.build/`

**Note**: First run downloads ~10GB of Docker images (one-time). Older compilers may fail with static linking.

### 2. Run Benchmarks (on host!)

```bash
./benchmarks.sh ilp_data_dependencies
```

Binaries are built in Docker but **run natively on host** for accurate performance measurements.

Results saved to `isolated_builds/.benchmarks/ilp_data_dependencies/benchmark.log`

### 3. Generate Disassembly

```bash
./disassembly.sh ilp_data_dependencies
```

Output saved to `isolated_builds/.disassembly/ilp_data_dependencies/`

### 4. Run Everything (all projects)

```bash
./run_all.sh
```

Builds, benchmarks, and generates disassembly for all projects at once.

## Compiler Versions

- **GCC**: 7, 8, 9, 10, 11, 12, 13
- **Clang**: 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18

Total: 20 compiler versions × 4 optimization levels = 80 builds per project

## Notes

- Binaries are **statically linked** to avoid glibc version issues
- Docker is only used for **compilation**, not execution
- Results are kept separate from native builds (main `.build/`, `.benchmarks/`)
- Make sure Docker is installed and running

## Comparison with Native Build

| | Native Build | Isolated Build |
|---|---|---|
| **Location** | Root directory | `isolated_builds/` |
| **Compilers** | Host-installed | Any version via Docker |
| **Scripts** | `benchmarks.sh` | `benchmarks.sh` (in isolated_builds/) |
| **Results** | `.benchmarks/` | `isolated_builds/.benchmarks/` |
| **Use case** | Development | Compiler comparison |
