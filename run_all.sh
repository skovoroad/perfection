#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

PROJECTS=("inlining" "virtual" "noexcept" "exception" "cache_locality" "branch_prediction" "ilp_no_data_dependencies")

echo "============================================"
echo "Running all benchmarks and disassembly"
echo "Projects: ${PROJECTS[@]}"
echo "============================================"
echo ""

for project in "${PROJECTS[@]}"; do
    echo "============================================"
    echo "Processing project: ${project}"
    echo "============================================"
    echo ""
    
    echo ">>> Running benchmarks for ${project}..."
    "${SCRIPT_DIR}/benchmarks.sh" "${project}"
    echo ""
    
    echo ">>> Running disassembly for ${project}..."
    "${SCRIPT_DIR}/disassembly.sh" "${project}"
    echo ""
    
    echo "============================================"
    echo "Completed: ${project}"
    echo "============================================"
    echo ""
done

echo "============================================"
echo "All projects processed successfully!"
echo "============================================"
echo ""
echo "Results:"
echo "  Benchmarks: ${SCRIPT_DIR}/.benchmarks/"
echo "  Disassembly: ${SCRIPT_DIR}/.disassembly/"
echo ""
echo "To view results:"
for project in "${PROJECTS[@]}"; do
    echo "  cat ${SCRIPT_DIR}/.benchmarks/${project}_benchmark.log"
    echo "  ls ${SCRIPT_DIR}/.disassembly/${project}_*.dis"
done
