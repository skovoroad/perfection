#!/bin/bash

# Run all isolated build tasks (build, benchmarks, disassembly) for all projects

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

PROJECTS=("inlining" "virtual" "noexcept" "exception" "cache_locality" "branch_prediction" "ilp_no_data_dependencies" "ilp_data_dependencies")

echo "============================================"
echo "Running all isolated builds tasks"
echo "Projects: ${PROJECTS[@]}"
echo "============================================"
echo ""

for project in "${PROJECTS[@]}"; do
    echo "============================================"
    echo "Processing project: ${project}"
    echo "============================================"
    echo ""
    
    echo "--- Building with Docker ---"
    "${SCRIPT_DIR}/build.sh" "${project}"
    echo ""
    
    echo "--- Running benchmarks ---"
    "${SCRIPT_DIR}/benchmarks.sh" "${project}"
    echo ""
    
    echo "--- Generating disassembly ---"
    "${SCRIPT_DIR}/disassembly.sh" "${project}"
    echo ""
    
    echo "✓ Completed: ${project}"
    echo ""
done

echo "============================================"
echo "All projects processed!"
echo "============================================"
echo ""
echo "Results:"
echo "  Benchmarks: ${SCRIPT_DIR}/.benchmarks/"
echo "  Disassembly: ${SCRIPT_DIR}/.disassembly/"
