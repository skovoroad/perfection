#!/bin/bash

# Common build functions for Perfection framework
# This file is sourced by benchmarks.sh and disassembly.sh

set -e

# Build a project with specific compiler and optimization level
# Usage: build_project <project_dir> <build_dir> <compiler> <opt_level>
build_project() {
    local project_dir="$1"
    local build_dir="$2"
    local compiler="$3"
    local opt_level="$4"
    
    cmake -S "${project_dir}" -B "${build_dir}" \
        -DCOMPILER_CHOICE="${compiler}" \
        -DOPTIMIZATION_LEVEL="${opt_level}"
    cmake --build "${build_dir}"
}

# Build all configurations for a project
# Usage: build_all_configurations <project_name> <script_dir> [compilers] [opt_levels]
build_all_configurations() {
    local project_name="$1"
    local script_dir="$2"
    local compilers=("${3:-clang}" "${4:-gcc}")
    local opt_levels=("${5:-O0}" "${6:-O1}" "${7:-O2}" "${8:-O3}")
    
    # If compilers and opt_levels are not provided, use defaults
    if [ $# -lt 3 ]; then
        compilers=("clang" "gcc")
        opt_levels=("O0" "O1" "O2" "O3")
    fi
    
    local project_dir="${script_dir}/${project_name}"
    local build_base_dir="${script_dir}/.build"
    
    for compiler in "${compilers[@]}"; do
        for opt_level in "${opt_levels[@]}"; do
            local build_dir="${build_base_dir}/${project_name}/${compiler}_${opt_level}"
            
            echo "============================================"
            echo "Building ${project_name} with ${compiler} -${opt_level}..."
            echo "Build dir: ${build_dir}"
            echo "============================================"
            
            build_project "${project_dir}" "${build_dir}" "${compiler}" "${opt_level}"
            
            echo ""
        done
    done
}
