#include <benchmark/benchmark.h>
#include <map>

// Boost containers
#include <boost/container/flat_map.hpp>

// Abseil container
#include <absl/container/btree_map.h>

#include "common.h"

// =============================================================================
// ITERATE Benchmarks - Small Map (16 elements)
// =============================================================================

template<typename Container>
static void BM_Iterate_SmallMap(benchmark::State& state) {
    Container map;
    for (int i = 0; i < SMALL_MAP_SIZE; ++i) {
        map[Key(i)] = Value(i);
    }
    
    for (auto _ : state) {
        Value sum = 0;
        for (const auto& [k, v] : map) {
            sum += v;
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();  // Prevent loop hoisting/caching optimizations
    }
}

BENCHMARK(BM_Iterate_SmallMap<std::map<Key, Value>>)->Name("Iterate/SmallMap/StdMap");
BENCHMARK(BM_Iterate_SmallMap<boost::container::flat_map<Key, Value>>)->Name("Iterate/SmallMap/BoostFlatMap");
BENCHMARK(BM_Iterate_SmallMap<absl::btree_map<Key, Value>>)->Name("Iterate/SmallMap/AbslBtreeMap");

// =============================================================================
// ITERATE Benchmarks - Medium Map (256 elements)
// =============================================================================

template<typename Container>
static void BM_Iterate_MediumMap(benchmark::State& state) {
    Container map;
    for (int i = 0; i < MEDIUM_MAP_SIZE; ++i) {
        map[Key(i)] = Value(i);
    }
    
    for (auto _ : state) {
        Value sum = 0;
        for (const auto& [k, v] : map) {
            sum += v;
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();  // Prevent loop hoisting/caching optimizations
    }
}

BENCHMARK(BM_Iterate_MediumMap<std::map<Key, Value>>)->Name("Iterate/MediumMap/StdMap");
BENCHMARK(BM_Iterate_MediumMap<boost::container::flat_map<Key, Value>>)->Name("Iterate/MediumMap/BoostFlatMap");
BENCHMARK(BM_Iterate_MediumMap<absl::btree_map<Key, Value>>)->Name("Iterate/MediumMap/AbslBtreeMap");

// =============================================================================
// ITERATE Benchmarks - Large Map (4096 elements)
// =============================================================================

template<typename Container>
static void BM_Iterate_LargeMap(benchmark::State& state) {
    Container map;
    for (int i = 0; i < LARGE_MAP_SIZE; ++i) {
        map[Key(i)] = Value(i);
    }
    
    for (auto _ : state) {
        Value sum = 0;
        for (const auto& [k, v] : map) {
            sum += v;
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();  // Prevent loop hoisting/caching optimizations
    }
}

BENCHMARK(BM_Iterate_LargeMap<std::map<Key, Value>>)->Name("Iterate/LargeMap/StdMap");
BENCHMARK(BM_Iterate_LargeMap<boost::container::flat_map<Key, Value>>)->Name("Iterate/LargeMap/BoostFlatMap");
BENCHMARK(BM_Iterate_LargeMap<absl::btree_map<Key, Value>>)->Name("Iterate/LargeMap/AbslBtreeMap");

BENCHMARK_MAIN();
