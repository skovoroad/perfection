#!/bin/bash

set -e

# Source common build functions
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/build_common.sh"

if [ $# -ne 1 ]; then
    echo "Usage: $0 <project_name>"
    echo "Example: $0 inlining"
    exit 1
fi

PROJECT_NAME="$1"
PROJECT_DIR="${SCRIPT_DIR}/${PROJECT_NAME}"
# For nested projects (e.g., containers/vector), use the last component as binary name
BINARY_NAME=$(basename "${PROJECT_NAME}")
BUILD_BASE_DIR="${SCRIPT_DIR}/.build"
# Replace slashes with underscores for directory names
SAFE_PROJECT_NAME="${PROJECT_NAME//\//_}"
BENCHMARKS_DIR="${SCRIPT_DIR}/.benchmarks/${SAFE_PROJECT_NAME}"
BENCHMARK_FILE="${BENCHMARKS_DIR}/benchmark.log"

if [ ! -d "${PROJECT_DIR}" ]; then
    echo "Error: Project directory ${PROJECT_DIR} does not exist"
    exit 1
fi

COMPILERS=("clang" "gcc")
OPT_LEVELS=("O0" "O1" "O2" "O3")

mkdir -p "${BENCHMARKS_DIR}"

> "${BENCHMARK_FILE}"

echo "============================================"
echo "Starting benchmark runs for project: ${PROJECT_NAME}"
echo "Compilers: ${COMPILERS[@]}"
echo "Optimization levels: ${OPT_LEVELS[@]}"
echo "============================================"
echo ""

for compiler in "${COMPILERS[@]}"; do
    for opt_level in "${OPT_LEVELS[@]}"; do
        BUILD_DIR="${BUILD_BASE_DIR}/${SAFE_PROJECT_NAME}/${compiler}_${opt_level}"
        
        echo "============================================"
        echo "Building ${PROJECT_NAME} with ${compiler} -${opt_level}..."
        echo "Build dir: ${BUILD_DIR}"
        echo "============================================"
        
        build_project "${PROJECT_DIR}" "${BUILD_DIR}" "${compiler}" "${opt_level}"
        
        echo ""
        echo "============================================"
        echo "Running benchmarks with ${compiler} -${opt_level}..."
        echo "============================================"
        
        echo "========== ${compiler} -${opt_level} ==========" >> "${BENCHMARK_FILE}"
        
        # Check if we have multiple bench_* binaries (new structure)
        BENCH_BINARIES=$(find "${BUILD_DIR}" -maxdepth 1 -name "bench_*" -type f 2>/dev/null || true)
        
        if [ -n "${BENCH_BINARIES}" ]; then
            # Multiple binaries - run each one
            for bench_binary in ${BENCH_BINARIES}; do
                BENCH_NAME=$(basename "${bench_binary}")
                echo "--- ${BENCH_NAME} ---" >> "${BENCHMARK_FILE}"
                "${bench_binary}" 2>&1 | grep -E "^(Benchmark|[A-Z][a-z]+/|---)" >> "${BENCHMARK_FILE}"
                echo "" >> "${BENCHMARK_FILE}"
            done
        else
            # Single binary (fallback to old behavior)
            "${BUILD_DIR}/${BINARY_NAME}" 2>&1 | grep -E "^(Benchmark|BM_|---)" >> "${BENCHMARK_FILE}"
        fi
        
        echo "" >> "${BENCHMARK_FILE}"
        
        echo ""
    done
done

echo "============================================"
echo "Done! Results saved to: ${BENCHMARK_FILE}"
echo "============================================"
echo ""
echo "To view results:"
echo "  cat ${BENCHMARK_FILE}"
echo ""
echo "Summary of test configurations:"
for compiler in "${COMPILERS[@]}"; do
    for opt_level in "${OPT_LEVELS[@]}"; do
        echo "  - ${compiler} -${opt_level}"
    done
done
