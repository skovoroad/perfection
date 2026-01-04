#!/bin/bash

# Run benchmarks built with Docker on host machine
# Uses statically-linked binaries from docker/.build/

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/.build"
BENCHMARKS_DIR="${SCRIPT_DIR}/.benchmarks"

if [ $# -ne 1 ]; then
    echo "Usage: $0 <project_name>"
    echo "Example: $0 ilp_data_dependencies"
    exit 1
fi

PROJECT_NAME="$1"
PROJECT_BUILD_DIR="${BUILD_DIR}/${PROJECT_NAME}"

if [ ! -d "${PROJECT_BUILD_DIR}" ]; then
    echo "Error: No Docker builds found for ${PROJECT_NAME}"
    echo "Run: ./build_docker.sh ${PROJECT_NAME} first"
    exit 1
fi

OUTPUT_DIR="${BENCHMARKS_DIR}/${PROJECT_NAME}"
mkdir -p "${OUTPUT_DIR}"

LOG_FILE="${OUTPUT_DIR}/benchmark.log"
> "${LOG_FILE}"

echo "============================================"
echo "Running Docker-built benchmarks for: ${PROJECT_NAME}"
echo "============================================"
echo ""

# Find all built binaries
for binary in "${PROJECT_BUILD_DIR}"/*/"${PROJECT_NAME}"; do
    if [ -f "${binary}" ] && [ -x "${binary}" ]; then
        # Extract compiler and opt level from path
        config=$(basename $(dirname "${binary}"))
        
        echo "========== ${config} ==========" | tee -a "${LOG_FILE}"
        
        # Run binary on host
        "${binary}" 2>&1 | tee -a "${LOG_FILE}"
        
        echo "" | tee -a "${LOG_FILE}"
    fi
done

echo "============================================"
echo "Done! Results saved to: ${LOG_FILE}"
echo "============================================"
echo ""
echo "To view results:"
echo "  cat ${LOG_FILE}"
