#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/.build"
LOG_DIR="${SCRIPT_DIR}/.log"
LOG_FILE="${LOG_DIR}/benchmark.log"

COMPILERS=("clang" "gcc")

OPT_LEVELS=("O0" "O1" "O2" "O3")

mkdir -p "${LOG_DIR}"

> "${LOG_FILE}"

echo "============================================"
echo "Starting benchmark runs..."
echo "Compilers: ${COMPILERS[@]}"
echo "Optimization levels: ${OPT_LEVELS[@]}"
echo "============================================"
echo ""

for compiler in "${COMPILERS[@]}"; do
    for opt_level in "${OPT_LEVELS[@]}"; do
        echo "============================================"
        echo "Building with ${compiler} -${opt_level}..."
        echo "============================================"
        
        rm -rf "${BUILD_DIR}"
        
        cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" \
            -DCOMPILER_CHOICE="${compiler}" \
            -DOPTIMIZATION_LEVEL="${opt_level}"
        cmake --build "${BUILD_DIR}"
        
        echo ""
        echo "============================================"
        echo "Running benchmarks with ${compiler} -${opt_level}..."
        echo "============================================"
        
        echo "========== ${compiler} -${opt_level} ==========" >> "${LOG_FILE}"
        "${BUILD_DIR}/inlining" >> "${LOG_FILE}" 2>&1
        echo "" >> "${LOG_FILE}"
        
        echo ""
    done
done

echo "============================================"
echo "Done! Results saved to: ${LOG_FILE}"
echo "============================================"
echo ""
echo "To view results:"
echo "  cat ${LOG_FILE}"
echo ""
echo "Summary of test configurations:"
for compiler in "${COMPILERS[@]}"; do
    for opt_level in "${OPT_LEVELS[@]}"; do
        echo "  - ${compiler} -${opt_level}"
    done
done
