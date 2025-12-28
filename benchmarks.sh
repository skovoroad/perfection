#!/bin/bash

set -e

if [ $# -ne 1 ]; then
    echo "Usage: $0 <project_name>"
    echo "Example: $0 inlining"
    exit 1
fi

PROJECT_NAME="$1"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="${SCRIPT_DIR}/${PROJECT_NAME}"
BUILD_BASE_DIR="${SCRIPT_DIR}/.build"
BENCHMARKS_DIR="${SCRIPT_DIR}/.benchmarks/${PROJECT_NAME}"
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
        BUILD_DIR="${BUILD_BASE_DIR}/${PROJECT_NAME}/${compiler}_${opt_level}"
        
        echo "============================================"
        echo "Building ${PROJECT_NAME} with ${compiler} -${opt_level}..."
        echo "Build dir: ${BUILD_DIR}"
        echo "============================================"
        
        cmake -S "${PROJECT_DIR}" -B "${BUILD_DIR}" \
            -DCOMPILER_CHOICE="${compiler}" \
            -DOPTIMIZATION_LEVEL="${opt_level}"
        cmake --build "${BUILD_DIR}"
        
        echo ""
        echo "============================================"
        echo "Running benchmarks with ${compiler} -${opt_level}..."
        echo "============================================"
        
        echo "========== ${compiler} -${opt_level} ==========" >> "${BENCHMARK_FILE}"
        "${BUILD_DIR}/${PROJECT_NAME}" 2>&1 | grep -E "^(Benchmark|BM_|---)" >> "${BENCHMARK_FILE}"
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
