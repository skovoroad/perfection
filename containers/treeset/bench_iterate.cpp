#include <benchmark/benchmark.h>
#include <set>

// Boost containers
#include <boost/container/flat_set.hpp>

// Abseil container
#include <absl/container/btree_set.h>

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

BENCHMARK(BM_Iterate_SmallSet<std::set<Element>>)->Name("Iterate/SmallSet/StdSet");
BENCHMARK(BM_Iterate_SmallSet<boost::container::flat_set<Element>>)->Name("Iterate/SmallSet/BoostFlatSet");
BENCHMARK(BM_Iterate_SmallSet<absl::btree_set<Element>>)->Name("Iterate/SmallSet/AbslBtreeSet");

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

BENCHMARK(BM_Iterate_MediumSet<std::set<Element>>)->Name("Iterate/MediumSet/StdSet");
BENCHMARK(BM_Iterate_MediumSet<boost::container::flat_set<Element>>)->Name("Iterate/MediumSet/BoostFlatSet");
BENCHMARK(BM_Iterate_MediumSet<absl::btree_set<Element>>)->Name("Iterate/MediumSet/AbslBtreeSet");

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

BENCHMARK(BM_Iterate_LargeSet<std::set<Element>>)->Name("Iterate/LargeSet/StdSet");
BENCHMARK(BM_Iterate_LargeSet<boost::container::flat_set<Element>>)->Name("Iterate/LargeSet/BoostFlatSet");
BENCHMARK(BM_Iterate_LargeSet<absl::btree_set<Element>>)->Name("Iterate/LargeSet/AbslBtreeSet");

BENCHMARK_MAIN();
