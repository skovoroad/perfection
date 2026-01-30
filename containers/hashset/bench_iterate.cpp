#include <benchmark/benchmark.h>
#include <unordered_set>

// Abseil container
#include <absl/container/flat_hash_set.h>

#include "common.h"

// =============================================================================
// ITERATE Benchmarks - Small Set (16 elements)
// =============================================================================

template<typename Container>
static void BM_Iterate_SmallSet(benchmark::State& state) {
    Container set;
    for (int i = 0; i < SMALL_SET_SIZE; ++i) {
        set.insert(Element(i));
    }
    
    for (auto _ : state) {
        Element sum = 0;
        for (const auto& elem : set) {
            sum += elem;
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();  // Prevent loop hoisting/caching optimizations
    }
}

BENCHMARK(BM_Iterate_SmallSet<std::unordered_set<Element>>)->Name("Iterate/SmallSet/StdUnorderedSet");
BENCHMARK(BM_Iterate_SmallSet<absl::flat_hash_set<Element>>)->Name("Iterate/SmallSet/AbslFlatHashSet");

// =============================================================================
// ITERATE Benchmarks - Medium Set (256 elements)
// =============================================================================

template<typename Container>
static void BM_Iterate_MediumSet(benchmark::State& state) {
    Container set;
    for (int i = 0; i < MEDIUM_SET_SIZE; ++i) {
        set.insert(Element(i));
    }
    
    for (auto _ : state) {
        Element sum = 0;
        for (const auto& elem : set) {
            sum += elem;
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();  // Prevent loop hoisting/caching optimizations
    }
}

BENCHMARK(BM_Iterate_MediumSet<std::unordered_set<Element>>)->Name("Iterate/MediumSet/StdUnorderedSet");
BENCHMARK(BM_Iterate_MediumSet<absl::flat_hash_set<Element>>)->Name("Iterate/MediumSet/AbslFlatHashSet");

// =============================================================================
// ITERATE Benchmarks - Large Set (4096 elements)
// =============================================================================

template<typename Container>
static void BM_Iterate_LargeSet(benchmark::State& state) {
    Container set;
    for (int i = 0; i < LARGE_SET_SIZE; ++i) {
        set.insert(Element(i));
    }
    
    for (auto _ : state) {
        Element sum = 0;
        for (const auto& elem : set) {
            sum += elem;
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();  // Prevent loop hoisting/caching optimizations
    }
}

BENCHMARK(BM_Iterate_LargeSet<std::unordered_set<Element>>)->Name("Iterate/LargeSet/StdUnorderedSet");
BENCHMARK(BM_Iterate_LargeSet<absl::flat_hash_set<Element>>)->Name("Iterate/LargeSet/AbslFlatHashSet");

BENCHMARK_MAIN();
