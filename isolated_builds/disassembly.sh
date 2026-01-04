#!/bin/bash

# Generate disassembly from Docker-built binaries

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/.build"
DISASM_DIR="${SCRIPT_DIR}/.disassembly"

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

OUTPUT_DIR="${DISASM_DIR}/${PROJECT_NAME}"
mkdir -p "${OUTPUT_DIR}"

echo "============================================"
echo "Generating disassembly for: ${PROJECT_NAME}"
echo "============================================"
echo ""

for binary in "${PROJECT_BUILD_DIR}"/*/"${PROJECT_NAME}"; do
    if [ -f "${binary}" ] && [ -x "${binary}" ]; then
        config=$(basename $(dirname "${binary}"))
        disasm_file="${OUTPUT_DIR}/${config}.dis"
        
        echo "Processing ${config}..."
        
        # Full disassembly
        objdump -d -C "${binary}" > "${OUTPUT_DIR}/full_${config}.dis"
        
        # Extract prfct_ functions
        > "${disasm_file}"
        echo "Project: ${PROJECT_NAME}" >> "${disasm_file}"
        echo "Configuration: ${config}" >> "${disasm_file}"
        echo "" >> "${disasm_file}"
        
        FUNCTIONS=$(grep -E '^[0-9a-f]+ <.*prfct_.*>' "${OUTPUT_DIR}/full_${config}.dis" | \
                    sed 's/^[0-9a-f]\+ <\(.*\)>:/\1/')
        
        while IFS= read -r funcname; do
            if [ -n "$funcname" ]; then
                echo "========== $funcname ==========" >> "${disasm_file}"
                in_function=0
                while IFS= read -r line; do
                    if [[ "$line" == *"<${funcname}>:"* ]]; then
                        in_function=1
                    fi
                    if [ $in_function -eq 1 ]; then
                        echo "$line" | sed 's/^[ ]*[0-9a-f]\+://' >> "${disasm_file}"
                        if [ -z "$line" ]; then
                            break
                        fi
                    fi
                done < "${OUTPUT_DIR}/full_${config}.dis"
                echo "" >> "${disasm_file}"
            fi
        done <<< "$FUNCTIONS"
        
        rm "${OUTPUT_DIR}/full_${config}.dis"
        echo "Saved to: ${disasm_file}"
    fi
done

echo ""
echo "============================================"
echo "Done! Disassembly files saved to: ${OUTPUT_DIR}/"
echo "============================================"
