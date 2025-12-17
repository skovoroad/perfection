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
BUILD_DIR="${PROJECT_DIR}/.build"
DISASM_DIR="${SCRIPT_DIR}/.disassembly"

if [ ! -d "${PROJECT_DIR}" ]; then
    echo "Error: Project directory ${PROJECT_DIR} does not exist"
    exit 1
fi

COMPILERS=("clang" "gcc")
OPT_LEVELS=("O0" "O1" "O2" "O3")

mkdir -p "${DISASM_DIR}"

echo "============================================"
echo "Starting disassembly runs for project: ${PROJECT_NAME}"
echo "Compilers: ${COMPILERS[@]}"
echo "Optimization levels: ${OPT_LEVELS[@]}"
echo "============================================"
echo ""

for compiler in "${COMPILERS[@]}"; do
    for opt_level in "${OPT_LEVELS[@]}"; do
        EXECUTABLE="${BUILD_DIR}/${PROJECT_NAME}"
        DISASM_FILE="${DISASM_DIR}/${PROJECT_NAME}_${compiler}_${opt_level}.dis"
        
        echo "============================================"
        echo "Building ${PROJECT_NAME} with ${compiler} -${opt_level}..."
        echo "============================================"
        
        rm -rf "${BUILD_DIR}"
        
        cmake -S "${PROJECT_DIR}" -B "${BUILD_DIR}" \
            -DCOMPILER_CHOICE="${compiler}" \
            -DOPTIMIZATION_LEVEL="${opt_level}"
        cmake --build "${BUILD_DIR}"
        
        echo ""
        echo "============================================"
        echo "Disassembling with ${compiler} -${opt_level}..."
        echo "============================================"
        
        objdump -d -C "${EXECUTABLE}" > "${DISASM_DIR}/full-${PROJECT_NAME}-${compiler}-${opt_level}.dis"
        
        > "${DISASM_FILE}"
        
        echo "Project: ${PROJECT_NAME}" >> "${DISASM_FILE}"
        echo "Compiler: ${compiler}" >> "${DISASM_FILE}"
        echo "Optimization: -${opt_level}" >> "${DISASM_FILE}"
        echo "" >> "${DISASM_FILE}"
        
        echo "========== process_random_data_inlined ==========" >> "${DISASM_FILE}"
        awk '/^[0-9a-f]+ <.*process_random_data_inlined.*>:/,/^$/' "${DISASM_DIR}/full-${PROJECT_NAME}-${compiler}-${opt_level}.dis" >> "${DISASM_FILE}"
        echo "" >> "${DISASM_FILE}"
        
        echo "========== process_random_data_noinline ==========" >> "${DISASM_FILE}"
        awk '/^[0-9a-f]+ <.*process_random_data_noinline.*>:/,/^$/' "${DISASM_DIR}/full-${PROJECT_NAME}-${compiler}-${opt_level}.dis" >> "${DISASM_FILE}"
        echo "" >> "${DISASM_FILE}"
        
        echo "========== swap_chars_noinline ==========" >> "${DISASM_FILE}"
        awk '/^[0-9a-f]+ <.*swap_chars_noinline.*>:/,/^$/' "${DISASM_DIR}/full-${PROJECT_NAME}-${compiler}-${opt_level}.dis" >> "${DISASM_FILE}"
        echo "" >> "${DISASM_FILE}"
        
        rm "${DISASM_DIR}/full-${PROJECT_NAME}-${compiler}-${opt_level}.dis"
        
        echo "Saved to: ${DISASM_FILE}"
        echo ""
    done
done

echo "============================================"
echo "Done! Disassembly files saved to: ${DISASM_DIR}/"
echo "============================================"
echo ""
echo "Generated files:"
for compiler in "${COMPILERS[@]}"; do
    for opt_level in "${OPT_LEVELS[@]}"; do
        echo "  - ${PROJECT_NAME}_${compiler}_${opt_level}.dis"
    done
done
echo ""
echo "To view a specific disassembly:"
echo "  cat ${DISASM_DIR}/${PROJECT_NAME}_clang_O3.dis"
echo "  or"
echo "  less ${DISASM_DIR}/${PROJECT_NAME}_gcc_O2.dis"
