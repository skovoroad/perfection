#!/bin/bash
# Generate summary.md for all benchmark projects

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BENCHMARKS_DIR="${SCRIPT_DIR}/.benchmarks"

if [ ! -d "${BENCHMARKS_DIR}" ]; then
    echo "Error: .benchmarks directory not found"
    exit 1
fi

echo "Generating summaries for all projects..."
echo ""

generated_count=0
skipped_count=0

# Iterate through all project directories
for project_dir in "${BENCHMARKS_DIR}"/*/ ; do
    if [ ! -d "${project_dir}" ]; then
        continue
    fi
    
    project_name=$(basename "${project_dir}")
    benchmark_log="${project_dir}/benchmark.log"
    
    if [ ! -f "${benchmark_log}" ]; then
        echo "⊘ Skipped ${project_name}: no benchmark.log"
        skipped_count=$((skipped_count + 1))
        continue
    fi
    
    echo "→ Processing ${project_name}..."
    
    if python3 "${SCRIPT_DIR}/generate_summary.py" "${benchmark_log}"; then
        generated_count=$((generated_count + 1))
    else
        echo "  ✗ Failed to generate summary for ${project_name}"
    fi
done

echo ""
echo "============================================"
echo "Summary generation complete"
echo "  Generated: ${generated_count}"
echo "  Skipped:   ${skipped_count}"
echo "============================================"
