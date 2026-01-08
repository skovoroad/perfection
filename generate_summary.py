#!/usr/bin/env python3
"""
Generate summary.md from benchmark.log.

Extracts O3 results and creates comparison tables.
Supports both simple (BM_name) and hierarchical (Operation/Size/Container) naming.
"""

import re
import sys
from pathlib import Path
from collections import defaultdict
from typing import Dict, List, Tuple


def parse_benchmark_log(log_path: Path) -> Dict[str, Dict[str, str]]:
    """
    Parse benchmark.log and extract results.
    
    Returns:
        {
            'clang-O3': {'BM_test1': '123 ns', ...},
            'clang-O2': {'BM_test1': '125 ns', ...},
            'gcc-O3': {'BM_test1': '120 ns', ...},
            ...
        }
    """
    results = defaultdict(dict)
    current_config = None
    
    with open(log_path, 'r') as f:
        for line in f:
            # Detect configuration header: "========== clang -O3 =========="
            config_match = re.match(r'=+\s+(clang|gcc)\s+-O(\d)', line)
            if config_match:
                compiler = config_match.group(1)
                opt_level = config_match.group(2)
                current_config = f"{compiler}-O{opt_level}"
                continue
            
            # Process all optimization levels
            if current_config:
                # Match benchmark result line
                # Format: "BM_name     123 ns     123 ns    12345"
                # or: "Operation/Size/Container     123 ns     123 ns    12345"
                bench_match = re.match(r'(\S+)\s+[\d.]+\s+ns\s+([\d.]+)\s+ns', line)
                if bench_match:
                    bench_name = bench_match.group(1)
                    cpu_time_str = bench_match.group(2)
                    # Store with " ns" suffix for consistency
                    results[current_config][bench_name] = f"{cpu_time_str} ns"
    
    return dict(results)


def normalize_precision(values: List[str]) -> List[str]:
    """
    Normalize precision by padding with zeros to align decimal points.
    
    Input: ['16.5 ns', '0.990 ns', '2.03 ns', 'N/A']
    Output: ['16.500 ns', '0.990 ns', '2.030 ns', 'N/A']
    """
    # Extract numbers and find max decimal places
    max_decimals = 0
    
    for val in values:
        if val == "N/A":
            continue
        # Extract number part (before " ns")
        num_str = val.replace(" ns", "").strip()
        if '.' in num_str:
            decimals = len(num_str.split('.')[1])
            max_decimals = max(max_decimals, decimals)
    
    # Normalize all values
    normalized = []
    for val in values:
        if val == "N/A":
            normalized.append(val)
            continue
            
        num_str = val.replace(" ns", "").strip()
        
        if '.' in num_str:
            # Has decimal point - pad to max
            integer_part, decimal_part = num_str.split('.')
            padded = f"{integer_part}.{decimal_part:<{max_decimals}}" if max_decimals > 0 else integer_part
            normalized.append(f"{padded} ns")
        elif max_decimals > 0:
            # No decimal point but others have - add it
            normalized.append(f"{num_str}.{'0' * max_decimals} ns")
        else:
            # No decimals at all
            normalized.append(val)
    
    return normalized


def detect_naming_pattern(benchmarks: List[str]) -> str:
    """Detect if benchmarks use simple or hierarchical naming."""
    if not benchmarks:
        return 'simple'
    
    # Hierarchical names have at least one '/'
    hierarchical_count = sum(1 for b in benchmarks if '/' in b)
    
    return 'hierarchical' if hierarchical_count > len(benchmarks) / 2 else 'simple'


def group_hierarchical_benchmarks(benchmarks: Dict[str, str]) -> Dict[str, Dict[str, str]]:
    """
    Group hierarchical benchmarks by prefix.
    
    Input: {'Copy/Small_int/StdVector': '15 ns', 'Copy/Small_int/SmallVector': '3 ns'}
    Output: {'Copy/Small_int': {'StdVector': '15 ns', 'SmallVector': '3 ns'}}
    """
    groups = defaultdict(dict)
    
    for bench_name, time in benchmarks.items():
        parts = bench_name.rsplit('/', 1)
        if len(parts) == 2:
            group_name, container = parts
            groups[group_name][container] = time
        else:
            # Fallback for simple names
            groups['Other'][bench_name] = time
    
    return dict(groups)


def generate_simple_table(results: Dict[str, Dict[str, str]]) -> str:
    """Generate markdown table for simple benchmark names."""
    # Get all unique benchmark names
    all_benchmarks = set()
    for config_results in results.values():
        all_benchmarks.update(config_results.keys())
    
    if not all_benchmarks:
        return "No benchmarks found.\n"
    
    all_benchmarks = sorted(all_benchmarks)
    
    # Calculate column widths
    max_bench_width = max(len(bench) for bench in all_benchmarks)
    max_bench_width = max(max_bench_width, len("Benchmark"))
    
    # Define optimization levels in order (highest to lowest)
    opt_levels = ['O3', 'O2', 'O1', 'O0']
    compilers = ['clang', 'gcc']  # clang first for each level
    
    # Collect values for each config
    all_columns = {}
    for opt in opt_levels:
        for compiler in compilers:
            config = f"{compiler}-{opt}"
            column_values = []
            for bench in all_benchmarks:
                value = results.get(config, {}).get(bench, None)
                column_values.append(value if value else "N/A")
            # Normalize precision
            all_columns[config] = normalize_precision(column_values)
    
    # Calculate max width for each column
    column_widths = {}
    for config in all_columns:
        max_width = max(len(v) for v in all_columns[config])
        max_width = max(max_width, len(config))
        column_widths[config] = max_width
    
    # Build header
    md = f"| {'Benchmark':<{max_bench_width}} "
    for opt in opt_levels:
        for compiler in compilers:
            config = f"{compiler}-{opt}"
            md += f"| {config:>{column_widths[config]}} "
    md += "|\n"
    
    # Build separator
    md += f"|{'-' * (max_bench_width + 2)}"
    for opt in opt_levels:
        for compiler in compilers:
            config = f"{compiler}-{opt}"
            md += f"|{'-' * (column_widths[config] + 2)}"
    md += "|\n"
    
    # Build data rows
    for i, bench in enumerate(all_benchmarks):
        md += f"| {bench:<{max_bench_width}} "
        for opt in opt_levels:
            for compiler in compilers:
                config = f"{compiler}-{opt}"
                value = all_columns[config][i]
                md += f"| {value:>{column_widths[config]}} "
        md += "|\n"
    
    return md


def generate_hierarchical_tables(results: Dict[str, Dict[str, str]]) -> str:
    """Generate markdown tables for hierarchical benchmark names."""
    # Define optimization levels in order (highest to lowest)
    opt_levels = ['O3', 'O2', 'O1', 'O0']
    compilers = ['clang', 'gcc']  # clang first for each level
    
    # Group benchmarks for all configs
    all_groups_data = {}
    for opt in opt_levels:
        for compiler in compilers:
            config = f"{compiler}-{opt}"
            all_groups_data[config] = group_hierarchical_benchmarks(results.get(config, {}))
    
    # Get all unique group names across all configs
    all_groups = set()
    for groups in all_groups_data.values():
        all_groups.update(groups.keys())
    all_groups = sorted(all_groups)
    
    if not all_groups:
        return "No benchmarks found.\n"
    
    md = ""
    
    for group in all_groups:
        # Get all containers in this group across all configs
        all_containers = set()
        for groups in all_groups_data.values():
            if group in groups:
                all_containers.update(groups[group].keys())
        all_containers = sorted(all_containers)
        
        if not all_containers:
            continue
        
        # Add group header
        md += f"## {group}\n\n"
        
        # Calculate column widths
        max_container_width = max(len(c) for c in all_containers)
        max_container_width = max(max_container_width, len("Container"))
        
        # Collect values for each config
        all_columns = {}
        for opt in opt_levels:
            for compiler in compilers:
                config = f"{compiler}-{opt}"
                column_values = []
                group_data = all_groups_data[config].get(group, {})
                for container in all_containers:
                    value = group_data.get(container, None)
                    column_values.append(value if value else "N/A")
                # Normalize precision
                all_columns[config] = normalize_precision(column_values)
        
        # Calculate max width for each column
        column_widths = {}
        for config in all_columns:
            max_width = max(len(v) for v in all_columns[config])
            max_width = max(max_width, len(config))
            column_widths[config] = max_width
        
        # Build header
        md += f"| {'Container':<{max_container_width}} "
        for opt in opt_levels:
            for compiler in compilers:
                config = f"{compiler}-{opt}"
                md += f"| {config:>{column_widths[config]}} "
        md += "|\n"
        
        # Build separator
        md += f"|{'-' * (max_container_width + 2)}"
        for opt in opt_levels:
            for compiler in compilers:
                config = f"{compiler}-{opt}"
                md += f"|{'-' * (column_widths[config] + 2)}"
        md += "|\n"
        
        # Build data rows
        for i, container in enumerate(all_containers):
            md += f"| {container:<{max_container_width}} "
            for opt in opt_levels:
                for compiler in compilers:
                    config = f"{compiler}-{opt}"
                    value = all_columns[config][i]
                    md += f"| {value:>{column_widths[config]}} "
            md += "|\n"
        
        md += "\n"
    
    return md


def generate_summary(log_path: Path) -> str:
    """Generate summary markdown from benchmark log."""
    results = parse_benchmark_log(log_path)
    
    if not results:
        return "# Benchmark Summary\n\nNo O3 results found.\n"
    
    # Get all benchmark names
    all_benchmarks = set()
    for config_results in results.values():
        all_benchmarks.update(config_results.keys())
    
    # Detect naming pattern
    pattern = detect_naming_pattern(list(all_benchmarks))
    
    # Generate markdown
    md = "# Benchmark Summary\n\n"
    md += f"**Optimization Levels**: O3, O2, O1, O0\n\n"
    md += f"**Naming Pattern**: {pattern}\n\n"
    
    if pattern == 'hierarchical':
        md += generate_hierarchical_tables(results)
    else:
        md += generate_simple_table(results)
    
    return md


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <path_to_benchmark.log>")
        sys.exit(1)
    
    log_path = Path(sys.argv[1])
    
    if not log_path.exists():
        print(f"Error: File not found: {log_path}")
        sys.exit(1)
    
    # Generate summary
    summary = generate_summary(log_path)
    
    # Write to summary.md in same directory
    output_path = log_path.parent / "summary.md"
    with open(output_path, 'w') as f:
        f.write(summary)
    
    print(f"✓ Generated: {output_path}")


if __name__ == "__main__":
    main()
