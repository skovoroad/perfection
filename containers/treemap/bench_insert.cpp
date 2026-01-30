#include <benchmark/benchmark.h>
#include <map>

// Boost containers
#include <boost/container/flat_map.hpp>

// Abseil container
#include <absl/container/btree_map.h>

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

BENCHMARK(BM_Insert_SmallMap<std::map<Key, Value>>)->Name("Insert/SmallMap/StdMap");
BENCHMARK(BM_Insert_SmallMap<boost::container::flat_map<Key, Value>>)->Name("Insert/SmallMap/BoostFlatMap");
BENCHMARK(BM_Insert_SmallMap<absl::btree_map<Key, Value>>)->Name("Insert/SmallMap/AbslBtreeMap");

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

BENCHMARK(BM_Insert_MediumMap<std::map<Key, Value>>)->Name("Insert/MediumMap/StdMap");
BENCHMARK(BM_Insert_MediumMap<boost::container::flat_map<Key, Value>>)->Name("Insert/MediumMap/BoostFlatMap");
BENCHMARK(BM_Insert_MediumMap<absl::btree_map<Key, Value>>)->Name("Insert/MediumMap/AbslBtreeMap");

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

BENCHMARK(BM_Insert_LargeMap<std::map<Key, Value>>)->Name("Insert/LargeMap/StdMap");
BENCHMARK(BM_Insert_LargeMap<boost::container::flat_map<Key, Value>>)->Name("Insert/LargeMap/BoostFlatMap");
BENCHMARK(BM_Insert_LargeMap<absl::btree_map<Key, Value>>)->Name("Insert/LargeMap/AbslBtreeMap");

BENCHMARK_MAIN();
