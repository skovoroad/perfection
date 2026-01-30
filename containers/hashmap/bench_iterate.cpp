#include <benchmark/benchmark.h>
#include <unordered_map>

// Abseil container
#include <absl/container/flat_hash_map.h>

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

BENCHMARK(BM_Iterate_SmallMap<std::unordered_map<Key, Value>>)->Name("Iterate/SmallMap/StdUnorderedMap");
BENCHMARK(BM_Iterate_SmallMap<absl::flat_hash_map<Key, Value>>)->Name("Iterate/SmallMap/AbslFlatHashMap");

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

BENCHMARK(BM_Iterate_MediumMap<std::unordered_map<Key, Value>>)->Name("Iterate/MediumMap/StdUnorderedMap");
BENCHMARK(BM_Iterate_MediumMap<absl::flat_hash_map<Key, Value>>)->Name("Iterate/MediumMap/AbslFlatHashMap");

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

BENCHMARK(BM_Iterate_LargeMap<std::unordered_map<Key, Value>>)->Name("Iterate/LargeMap/StdUnorderedMap");
BENCHMARK(BM_Iterate_LargeMap<absl::flat_hash_map<Key, Value>>)->Name("Iterate/LargeMap/AbslFlatHashMap");

BENCHMARK_MAIN();
