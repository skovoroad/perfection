#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/.build"
DISASM_DIR="${SCRIPT_DIR}/.disassembly"
EXECUTABLE="${BUILD_DIR}/inlining"

COMPILERS=("clang" "gcc")
OPT_LEVELS=("O0" "O1" "O2" "O3")

mkdir -p "${DISASM_DIR}"

echo "============================================"
echo "Starting disassembly runs..."
echo "Compilers: ${COMPILERS[@]}"
echo "Optimization levels: ${OPT_LEVELS[@]}"
echo "============================================"
echo ""

for compiler in "${COMPILERS[@]}"; do
    for opt_level in "${OPT_LEVELS[@]}"; do
        DISASM_FILE="${DISASM_DIR}/inlining-${compiler}-${opt_level}.dis"
        
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
        echo "Disassembling with ${compiler} -${opt_level}..."
        echo "============================================"
        
        objdump -d -C "${EXECUTABLE}" > "${DISASM_DIR}/full-${compiler}-${opt_level}.dis"
        
        > "${DISASM_FILE}"
        
        echo "Compiler: ${compiler}" >> "${DISASM_FILE}"
        echo "Optimization: -${opt_level}" >> "${DISASM_FILE}"
        echo "" >> "${DISASM_FILE}"
        
        echo "========== process_random_data_inlined ==========" >> "${DISASM_FILE}"
        awk '/^[0-9a-f]+ <.*process_random_data_inlined.*>:/,/^$/' "${DISASM_DIR}/full-${compiler}-${opt_level}.dis" >> "${DISASM_FILE}"
        echo "" >> "${DISASM_FILE}"
        
        echo "========== process_random_data_noinline ==========" >> "${DISASM_FILE}"
        awk '/^[0-9a-f]+ <.*process_random_data_noinline.*>:/,/^$/' "${DISASM_DIR}/full-${compiler}-${opt_level}.dis" >> "${DISASM_FILE}"
        echo "" >> "${DISASM_FILE}"
        
        echo "========== swap_chars_noinline ==========" >> "${DISASM_FILE}"
        awk '/^[0-9a-f]+ <.*swap_chars_noinline.*>:/,/^$/' "${DISASM_DIR}/full-${compiler}-${opt_level}.dis" >> "${DISASM_FILE}"
        echo "" >> "${DISASM_FILE}"
        
        rm "${DISASM_DIR}/full-${compiler}-${opt_level}.dis"
        
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
        echo "  - inlining-${compiler}-${opt_level}.dis"
    done
done
echo ""
echo "To view a specific disassembly:"
echo "  cat ${DISASM_DIR}/inlining-clang-O3.dis"
echo "  or"
echo "  less ${DISASM_DIR}/inlining-gcc-O2.dis"
