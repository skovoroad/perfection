#include <benchmark/benchmark.h>
#include <map>
#include <vector>
#include <algorithm>
#include <random>

// Boost containers
#include <boost/container/flat_map.hpp>

// Abseil container
#include <absl/container/btree_map.h>

#include "common.h"

// =============================================================================
// Helper: create shuffled keys for random erase order
// =============================================================================

static std::vector<Key> make_shuffled_keys(int count) {
    std::vector<Key> keys(count);
    for (int i = 0; i < count; ++i) {
        keys[i] = Key(i);
    }
    std::shuffle(keys.begin(), keys.end(), std::mt19937{42});
    return keys;
}

// =============================================================================
// ERASE Benchmarks - Small Map (16 elements)
// =============================================================================

template<typename Container>
static void BM_Erase_SmallMap(benchmark::State& state) {
    auto keys = make_shuffled_keys(SMALL_MAP_SIZE);
    
    for (auto _ : state) {
        state.PauseTiming();
        Container map;
        for (int i = 0; i < SMALL_MAP_SIZE; ++i) {
            map[Key(i)] = Value(i);
        }
        state.ResumeTiming();
        
        for (Key key : keys) {
            map.erase(key);
        }
        benchmark::DoNotOptimize(map);
    }
}

BENCHMARK(BM_Erase_SmallMap<std::map<Key, Value>>)->Name("Erase/SmallMap/StdMap");
BENCHMARK(BM_Erase_SmallMap<boost::container::flat_map<Key, Value>>)->Name("Erase/SmallMap/BoostFlatMap");
BENCHMARK(BM_Erase_SmallMap<absl::btree_map<Key, Value>>)->Name("Erase/SmallMap/AbslBtreeMap");

// =============================================================================
// ERASE Benchmarks - Medium Map (256 elements)
// =============================================================================

template<typename Container>
static void BM_Erase_MediumMap(benchmark::State& state) {
    auto keys = make_shuffled_keys(MEDIUM_MAP_SIZE);
    
    for (auto _ : state) {
        state.PauseTiming();
        Container map;
        for (int i = 0; i < MEDIUM_MAP_SIZE; ++i) {
            map[Key(i)] = Value(i);
        }
        state.ResumeTiming();
        
        for (Key key : keys) {
            map.erase(key);
        }
        benchmark::DoNotOptimize(map);
    }
}

BENCHMARK(BM_Erase_MediumMap<std::map<Key, Value>>)->Name("Erase/MediumMap/StdMap");
BENCHMARK(BM_Erase_MediumMap<boost::container::flat_map<Key, Value>>)->Name("Erase/MediumMap/BoostFlatMap");
BENCHMARK(BM_Erase_MediumMap<absl::btree_map<Key, Value>>)->Name("Erase/MediumMap/AbslBtreeMap");

// =============================================================================
// ERASE Benchmarks - Large Map (4096 elements)
// =============================================================================

template<typename Container>
static void BM_Erase_LargeMap(benchmark::State& state) {
    auto keys = make_shuffled_keys(LARGE_MAP_SIZE);
    
    for (auto _ : state) {
        state.PauseTiming();
        Container map;
        for (int i = 0; i < LARGE_MAP_SIZE; ++i) {
            map[Key(i)] = Value(i);
        }
        state.ResumeTiming();
        
        for (Key key : keys) {
            map.erase(key);
        }
        benchmark::DoNotOptimize(map);
    }
}

BENCHMARK(BM_Erase_LargeMap<std::map<Key, Value>>)->Name("Erase/LargeMap/StdMap");
BENCHMARK(BM_Erase_LargeMap<boost::container::flat_map<Key, Value>>)->Name("Erase/LargeMap/BoostFlatMap");
BENCHMARK(BM_Erase_LargeMap<absl::btree_map<Key, Value>>)->Name("Erase/LargeMap/AbslBtreeMap");

BENCHMARK_MAIN();
