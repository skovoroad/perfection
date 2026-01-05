#!/bin/bash

# Quick test script - builds only with GCC 13
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "${SCRIPT_DIR}")"
BUILD_DIR="${SCRIPT_DIR}/.build"
PROJECT_NAME="inlining"

compiler_image="gcc:13"
compiler_name="gcc_13"
COMPILER_CMD="g++"
opt_level="O2"

OUTPUT_DIR="${BUILD_DIR}/${PROJECT_NAME}/${compiler_name}_${opt_level}"
OUTPUT_BIN="${OUTPUT_DIR}/${PROJECT_NAME}"

mkdir -p "${OUTPUT_DIR}"

echo "============================================"
echo "Testing build with ${compiler_image} -${opt_level}"
echo "Output: ${OUTPUT_BIN}"
echo "============================================"

# Build Google Benchmark inside Docker container if not already built
BENCHMARK_BUILD_DIR="${BUILD_DIR}/.benchmark_cache/${compiler_name}"
BENCHMARK_LIB="${BENCHMARK_BUILD_DIR}/src/libbenchmark.a"

if [ ! -f "${BENCHMARK_LIB}" ]; then
    echo "Building Google Benchmark with ${compiler_image}..."
    mkdir -p "${BENCHMARK_BUILD_DIR}"
    
    docker run --rm \
        -v "${PROJECT_ROOT}:/work" \
        -w "/work/benchmark" \
        "${compiler_image}" \
        bash -c "
            # Install cmake if not available
            if ! command -v cmake &> /dev/null; then
                echo 'CMake not found, trying to install...'
                apt-get update -qq && apt-get install -y -qq cmake make || \
                echo 'Warning: Could not install cmake'
            fi
            
            # Build benchmark
            mkdir -p /tmp/benchmark_build
            cd /tmp/benchmark_build
            cmake /work/benchmark \
                -DCMAKE_BUILD_TYPE=Release \
                -DBENCHMARK_ENABLE_TESTING=OFF \
                -DBENCHMARK_ENABLE_GTEST_TESTS=OFF \
                -DCMAKE_CXX_COMPILER=${COMPILER_CMD}
            make -j\$(nproc)
            
            # Copy built library to cache
            mkdir -p /work/isolated_builds/.build/.benchmark_cache/${compiler_name}/src
            cp src/libbenchmark.a /work/isolated_builds/.build/.benchmark_cache/${compiler_name}/src/
        "
    
    if [ ! -f "${BENCHMARK_LIB}" ]; then
        echo "✗ Failed to build Google Benchmark"
        exit 1
    fi
    echo "✓ Google Benchmark built successfully"
else
    echo "Using cached Google Benchmark library"
fi

# Build project
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
        -o "${CONTAINER_OUTPUT}"

if [ -f "${OUTPUT_BIN}" ]; then
    echo "✓ Built successfully"
    chmod +x "${OUTPUT_BIN}"
    ls -lh "${OUTPUT_BIN}"
    echo ""
    echo "Running binary to verify..."
    "${OUTPUT_BIN}" --benchmark_list
else
    echo "✗ Build failed"
    exit 1
fi
