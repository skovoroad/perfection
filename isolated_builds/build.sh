#!/bin/bash

# Historical build script - tests compiler evolution from 2017 to 2024
# Builds projects using wide range of compiler versions to observe optimization improvements
# Outputs statically-linked binaries to isolated_builds/.build/

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "${SCRIPT_DIR}")"
BUILD_DIR="${SCRIPT_DIR}/.build"

if [ $# -ne 1 ]; then
    echo "Usage: $0 <project_name>"
    echo "Example: $0 ilp_data_dependencies"
    echo ""
    echo "This script builds with historical compiler versions to observe optimization evolution."
    exit 1
fi

PROJECT_NAME="$1"
PROJECT_DIR="${PROJECT_ROOT}/${PROJECT_NAME}"

if [ ! -d "${PROJECT_DIR}" ]; then
    echo "Error: Project directory ${PROJECT_DIR} does not exist"
    exit 1
fi

# Historical compiler versions showing optimization evolution
# GCC 7-13 (2017-2023): major improvements in auto-vectorization and loop optimization
# Clang 6-18 (2018-2024): steady evolution of optimizer
COMPILERS=(
    "gcc:7"      # 2017 - First stable modern vectorizer
    "gcc:8"      # 2018 - Improved IPA optimizations
    "gcc:9"      # 2019 - Better loop nest optimization
    "gcc:10"     # 2020 - Enhanced auto-vectorization
    "gcc:11"     # 2021 - Modern baseline
    "gcc:12"     # 2022 - Incremental improvements
    "gcc:13"     # 2023 - Latest stable
    "clang:6"    # 2018 - C++17 support
    "clang:7"    # 2018 - Improved vectorization
    "clang:8"    # 2019 - Better optimization passes
    "clang:9"    # 2019 - Enhanced loop optimizations
    "clang:10"   # 2020 - Improved IPA
    "clang:11"   # 2020 - Better auto-vectorization
    "clang:12"   # 2021 - Modern baseline
    "clang:13"   # 2021 - Enhanced optimizations
    "clang:14"   # 2022 - Incremental improvements
    "clang:15"   # 2022 - Better code generation
    "clang:16"   # 2023 - Latest features
    "clang:17"   # 2023 - Enhanced performance
    "clang:18"   # 2024 - Current stable
)

OPT_LEVELS=("O0" "O1" "O2" "O3")

echo "============================================"
echo "Historical build for project: ${PROJECT_NAME}"
echo "Compilers: ${#COMPILERS[@]} versions"
echo "Optimization levels: ${OPT_LEVELS[@]}"
echo "Total builds: $((${#COMPILERS[@]} * ${#OPT_LEVELS[@]}))"
echo "============================================"
echo ""
echo "NOTE: First run will download ~10GB of Docker images"
echo "      This happens only once - images are cached"
echo ""

for compiler_image in "${COMPILERS[@]}"; do
    compiler_name=$(echo $compiler_image | tr ':' '_' | tr '/' '_')
    
    for opt_level in "${OPT_LEVELS[@]}"; do
        OUTPUT_DIR="${BUILD_DIR}/${PROJECT_NAME}/${compiler_name}_${opt_level}"
        OUTPUT_BIN="${OUTPUT_DIR}/${PROJECT_NAME}"
        
        mkdir -p "${OUTPUT_DIR}"
        
        echo "============================================"
        echo "Building with ${compiler_image} -${opt_level}..."
        echo "Output: ${OUTPUT_BIN}"
        echo "============================================"
        
        # Detect compiler type from image name
        if [[ $compiler_image == gcc* ]]; then
            COMPILER_CMD="g++"
        elif [[ $compiler_image == clang* ]]; then
            COMPILER_CMD="clang++"
        else
            echo "Unknown compiler image: ${compiler_image}"
            continue
        fi
        
        # Build in Docker with static linking
        docker run --rm \
            -v "${PROJECT_ROOT}:/work" \
            -w "/work/${PROJECT_NAME}" \
            "${compiler_image}" \
            ${COMPILER_CMD} -${opt_level} -static main.cpp \
                -I../benchmark/include \
                -L../benchmark/build/src \
                -lbenchmark -lpthread \
                -o "${OUTPUT_BIN}" \
            2>&1 | grep -v "warning:" || true
        
        if [ -f "${OUTPUT_BIN}" ]; then
            echo "✓ Built successfully"
            chmod +x "${OUTPUT_BIN}"
        else
            echo "✗ Build failed (may be incompatible with static linking)"
        fi
        echo ""
    done
done

echo "============================================"
echo "Done! Binaries saved to: ${BUILD_DIR}/${PROJECT_NAME}/"
echo "============================================"
echo ""
echo "To run benchmarks:"
echo "  cd isolated_builds && ./benchmarks.sh ${PROJECT_NAME}"
echo ""
echo "To analyze compiler evolution, compare results across versions"
