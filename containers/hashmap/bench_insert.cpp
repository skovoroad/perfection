#include <benchmark/benchmark.h>
#include <unordered_map>

// Abseil container
#include <absl/container/flat_hash_map.h>

#include "common.h"

// =============================================================================
// INSERT Benchmarks - Small Map (16 elements)
// =============================================================================

template<typename Container>
static void BM_Insert_SmallMap(benchmark::State& state) {
    for (auto _ : state) {
        Container map;
        for (int i = 0; i < SMALL_MAP_SIZE; ++i) {
            map[Key(i)] = Value(i);
        }
        benchmark::DoNotOptimize(map);
        benchmark::ClobberMemory();  // Prevent caching map state between iterations
    }
}

template<typename Container>
static void BM_Insert_SmallMap_Reserved(benchmark::State& state) {
    for (auto _ : state) {
        Container map;
        map.reserve(SMALL_MAP_SIZE);
        for (int i = 0; i < SMALL_MAP_SIZE; ++i) {
            map[Key(i)] = Value(i);
        }
        benchmark::DoNotOptimize(map);
        benchmark::ClobberMemory();  // Prevent caching map state between iterations
    }
}

BENCHMARK(BM_Insert_SmallMap<std::unordered_map<Key, Value>>)->Name("Insert/SmallMap/StdUnorderedMap");
BENCHMARK(BM_Insert_SmallMap<absl::flat_hash_map<Key, Value>>)->Name("Insert/SmallMap/AbslFlatHashMap");
BENCHMARK(BM_Insert_SmallMap_Reserved<std::unordered_map<Key, Value>>)->Name("Insert/SmallMap/StdUnorderedMap_Reserved");
BENCHMARK(BM_Insert_SmallMap_Reserved<absl::flat_hash_map<Key, Value>>)->Name("Insert/SmallMap/AbslFlatHashMap_Reserved");

// =============================================================================
// INSERT Benchmarks - Medium Map (256 elements)
// =============================================================================

template<typename Container>
static void BM_Insert_MediumMap(benchmark::State& state) {
    for (auto _ : state) {
        Container map;
        for (int i = 0; i < MEDIUM_MAP_SIZE; ++i) {
            map[Key(i)] = Value(i);
        }
        benchmark::DoNotOptimize(map);
        benchmark::ClobberMemory();  // Prevent caching map state between iterations
    }
}

template<typename Container>
static void BM_Insert_MediumMap_Reserved(benchmark::State& state) {
    for (auto _ : state) {
        Container map;
        map.reserve(MEDIUM_MAP_SIZE);
        for (int i = 0; i < MEDIUM_MAP_SIZE; ++i) {
            map[Key(i)] = Value(i);
        }
        benchmark::DoNotOptimize(map);
        benchmark::ClobberMemory();  // Prevent caching map state between iterations
    }
}

BENCHMARK(BM_Insert_MediumMap<std::unordered_map<Key, Value>>)->Name("Insert/MediumMap/StdUnorderedMap");
BENCHMARK(BM_Insert_MediumMap<absl::flat_hash_map<Key, Value>>)->Name("Insert/MediumMap/AbslFlatHashMap");
BENCHMARK(BM_Insert_MediumMap_Reserved<std::unordered_map<Key, Value>>)->Name("Insert/MediumMap/StdUnorderedMap_Reserved");
BENCHMARK(BM_Insert_MediumMap_Reserved<absl::flat_hash_map<Key, Value>>)->Name("Insert/MediumMap/AbslFlatHashMap_Reserved");

// =============================================================================
// INSERT Benchmarks - Large Map (4096 elements)
// =============================================================================

template<typename Container>
static void BM_Insert_LargeMap(benchmark::State& state) {
    for (auto _ : state) {
        Container map;
        for (int i = 0; i < LARGE_MAP_SIZE; ++i) {
            map[Key(i)] = Value(i);
        }
        benchmark::DoNotOptimize(map);
        benchmark::ClobberMemory();  // Prevent caching map state between iterations
    }
}

template<typename Container>
static void BM_Insert_LargeMap_Reserved(benchmark::State& state) {
    for (auto _ : state) {
        Container map;
        map.reserve(LARGE_MAP_SIZE);
        for (int i = 0; i < LARGE_MAP_SIZE; ++i) {
            map[Key(i)] = Value(i);
        }
        benchmark::DoNotOptimize(map);
        benchmark::ClobberMemory();  // Prevent caching map state between iterations
    }
}

BENCHMARK(BM_Insert_LargeMap<std::unordered_map<Key, Value>>)->Name("Insert/LargeMap/StdUnorderedMap");
BENCHMARK(BM_Insert_LargeMap<absl::flat_hash_map<Key, Value>>)->Name("Insert/LargeMap/AbslFlatHashMap");
BENCHMARK(BM_Insert_LargeMap_Reserved<std::unordered_map<Key, Value>>)->Name("Insert/LargeMap/StdUnorderedMap_Reserved");
BENCHMARK(BM_Insert_LargeMap_Reserved<absl::flat_hash_map<Key, Value>>)->Name("Insert/LargeMap/AbslFlatHashMap_Reserved");

BENCHMARK_MAIN();
