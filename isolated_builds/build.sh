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
    "gcc:7"              # 2017 - First stable modern vectorizer
    "gcc:8"              # 2018 - Improved IPA optimizations
    "gcc:9"              # 2019 - Better loop nest optimization
    "gcc:10"             # 2020 - Enhanced auto-vectorization
    "gcc:11"             # 2021 - Modern baseline
    "gcc:12"             # 2022 - Incremental improvements
    "gcc:13"             # 2023 - Latest stable
    "silkeh/clang:6"     # 2018 - C++17 support
    "silkeh/clang:7"     # 2018 - Improved vectorization
    "silkeh/clang:8"     # 2019 - Better optimization passes
    "silkeh/clang:9"     # 2019 - Enhanced loop optimizations
    "silkeh/clang:10"    # 2020 - Improved IPA
    "silkeh/clang:11"    # 2020 - Better auto-vectorization
    "silkeh/clang:12"    # 2021 - Modern baseline
    "silkeh/clang:13"    # 2021 - Enhanced optimizations
    "silkeh/clang:14"    # 2022 - Incremental improvements
    "silkeh/clang:15"    # 2022 - Better code generation
    "silkeh/clang:16"    # 2023 - Latest features
    "silkeh/clang:17"    # 2023 - Enhanced performance
    "silkeh/clang:18"    # 2024 - Current stable
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
        
        # Build Google Benchmark inside Docker container if not already built for this compiler
        BENCHMARK_BUILD_DIR="${BUILD_DIR}/.benchmark_cache/${compiler_name}"
        BENCHMARK_LIB="${BENCHMARK_BUILD_DIR}/src/libbenchmark.a"
        
        if [ ! -f "${BENCHMARK_LIB}" ]; then
            echo "Building Google Benchmark with ${compiler_image}..."
            mkdir -p "${BENCHMARK_BUILD_DIR}"
            
            # Build benchmark library inside Docker container
            docker run --rm \
                -v "${PROJECT_ROOT}:/work" \
                -w "/work/benchmark" \
                "${compiler_image}" \
                bash -c "
                    # Install cmake if not available
                    if ! command -v cmake &> /dev/null; then
                        echo 'CMake not found, trying to install...'
                        apt-get update -qq && apt-get install -y -qq cmake make 2>&1 || \
                        yum install -y -q cmake make 2>&1 || \
                        echo 'Warning: Could not install cmake, assuming it exists'
                    fi
                    
                    # Build benchmark
                    mkdir -p /tmp/benchmark_build
                    cd /tmp/benchmark_build
                    cmake /work/benchmark \
                        -DCMAKE_BUILD_TYPE=Release \
                        -DBENCHMARK_ENABLE_TESTING=OFF \
                        -DBENCHMARK_ENABLE_GTEST_TESTS=OFF \
                        -DCMAKE_CXX_COMPILER=${COMPILER_CMD} 2>&1
                    make -j\$(nproc) 2>&1
                    
                    # Copy built library to cache
                    mkdir -p /work/isolated_builds/.build/.benchmark_cache/${compiler_name}/src
                    cp src/libbenchmark.a /work/isolated_builds/.build/.benchmark_cache/${compiler_name}/src/
                " 2>&1 | grep -E "(Building|Built|Error|error)" || true
            
            if [ ! -f "${BENCHMARK_LIB}" ]; then
                echo "✗ Failed to build Google Benchmark for ${compiler_image}"
                echo "  Skipping all builds for this compiler"
                break  # Skip all optimization levels for this compiler
            fi
            echo "✓ Google Benchmark built successfully"
        fi
        
        # Build project using container-built benchmark library
        # Output path must be relative to /work mount in container
        CONTAINER_OUTPUT="/work/isolated_builds/.build/${PROJECT_NAME}/${compiler_name}_${opt_level}/${PROJECT_NAME}"
        CONTAINER_BENCHMARK_LIB="/work/isolated_builds/.build/.benchmark_cache/${compiler_name}/src/libbenchmark.a"
        
        docker run --rm \
            -v "${PROJECT_ROOT}:/work" \
            -w "/work/${PROJECT_NAME}" \
            "${compiler_image}" \
            ${COMPILER_CMD} -${opt_level} main.cpp \
                -I../benchmark/include \
                ${CONTAINER_BENCHMARK_LIB} \
                -lpthread \
                -o "${CONTAINER_OUTPUT}" \
            2>&1 | grep -v "warning:" || true
        
        if [ -f "${OUTPUT_BIN}" ]; then
            echo "✓ Built successfully"
        else
            echo "✗ Build failed"
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
