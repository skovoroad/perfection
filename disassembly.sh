#!/bin/bash

set -e

# Source common build functions
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/build_common.sh"

if [ $# -ne 1 ]; then
    echo "Usage: $0 <project_name>"
    echo "Example: $0 inlining"
    exit 1
fi

PROJECT_NAME="$1"
PROJECT_DIR="${SCRIPT_DIR}/${PROJECT_NAME}"
BUILD_BASE_DIR="${SCRIPT_DIR}/.build"
DISASM_DIR="${SCRIPT_DIR}/.disassembly/${PROJECT_NAME}"

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
        BUILD_DIR="${BUILD_BASE_DIR}/${PROJECT_NAME}/${compiler}_${opt_level}"
        EXECUTABLE="${BUILD_DIR}/${PROJECT_NAME}"
        DISASM_FILE="${DISASM_DIR}/${compiler}_${opt_level}.dis"
        
        echo "============================================"
        echo "Building ${PROJECT_NAME} with ${compiler} -${opt_level}..."
        echo "Build dir: ${BUILD_DIR}"
        echo "============================================"
        
        build_project "${PROJECT_DIR}" "${BUILD_DIR}" "${compiler}" "${opt_level}"
        
        echo ""
        echo "============================================"
        echo "Disassembling with ${compiler} -${opt_level}..."
        echo "============================================"
        
        objdump -d -C "${EXECUTABLE}" > "${DISASM_DIR}/full_${compiler}_${opt_level}.dis"
        
        > "${DISASM_FILE}"
        
        echo "Project: ${PROJECT_NAME}" >> "${DISASM_FILE}"
        echo "Compiler: ${compiler}" >> "${DISASM_FILE}"
        echo "Optimization: -${opt_level}" >> "${DISASM_FILE}"
        echo "" >> "${DISASM_FILE}"
        
        # Extract functions with prfct_ in the name (both regular and template functions)
        FUNCTIONS=$(grep -E '^[0-9a-f]+ <.*prfct_.*>' "${DISASM_DIR}/full_${compiler}_${opt_level}.dis" | \
                    sed 's/^[0-9a-f]\+ <\(.*\)>:/\1/')
        
        # Extract each relevant function
        while IFS= read -r funcname; do
            if [ -n "$funcname" ]; then
                echo "========== $funcname ==========" >> "${DISASM_FILE}"
                # Read file line by line, extract from function start to empty line
                in_function=0
                while IFS= read -r line; do
                    if [[ "$line" == *"<${funcname}>:"* ]]; then
                        in_function=1
                    fi
                    if [ $in_function -eq 1 ]; then
                        echo "$line" | sed 's/^[ ]*[0-9a-f]\+://' >> "${DISASM_FILE}"
                        if [ -z "$line" ]; then
                            break
                        fi
                    fi
                done < "${DISASM_DIR}/full_${compiler}_${opt_level}.dis"
                echo "" >> "${DISASM_FILE}"
            fi
        done <<< "$FUNCTIONS"
        
        rm "${DISASM_DIR}/full_${compiler}_${opt_level}.dis"
        
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
        echo "  - ${compiler}_${opt_level}.dis"
    done
done
echo ""
echo "To view a specific disassembly:"
echo "  cat ${DISASM_DIR}/clang_O3.dis"
echo "  or"
echo "  less ${DISASM_DIR}/${PROJECT_NAME}_gcc_O2.dis"
