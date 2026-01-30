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
// Helper: create and shuffle keys for random access
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
// LOOKUP Benchmarks - Small Map (16 elements)
// =============================================================================

template<typename Container>
static void BM_Lookup_SmallMap(benchmark::State& state) {
    Container map;
    for (int i = 0; i < SMALL_MAP_SIZE; ++i) {
        map[Key(i)] = Value(i);
    }
    auto keys = make_shuffled_keys(SMALL_MAP_SIZE);
    
    for (auto _ : state) {
        for (Key key : keys) {
            auto it = map.find(key);
            benchmark::DoNotOptimize(it);
        }
    }
}

BENCHMARK(BM_Lookup_SmallMap<std::map<Key, Value>>)->Name("Lookup/SmallMap/StdMap");
BENCHMARK(BM_Lookup_SmallMap<boost::container::flat_map<Key, Value>>)->Name("Lookup/SmallMap/BoostFlatMap");
BENCHMARK(BM_Lookup_SmallMap<absl::btree_map<Key, Value>>)->Name("Lookup/SmallMap/AbslBtreeMap");

// =============================================================================
// LOOKUP Benchmarks - Medium Map (256 elements)
// =============================================================================

template<typename Container>
static void BM_Lookup_MediumMap(benchmark::State& state) {
    Container map;
    for (int i = 0; i < MEDIUM_MAP_SIZE; ++i) {
        map[Key(i)] = Value(i);
    }
    auto keys = make_shuffled_keys(MEDIUM_MAP_SIZE);
    
    for (auto _ : state) {
        for (Key key : keys) {
            auto it = map.find(key);
            benchmark::DoNotOptimize(it);
        }
    }
}

BENCHMARK(BM_Lookup_MediumMap<std::map<Key, Value>>)->Name("Lookup/MediumMap/StdMap");
BENCHMARK(BM_Lookup_MediumMap<boost::container::flat_map<Key, Value>>)->Name("Lookup/MediumMap/BoostFlatMap");
BENCHMARK(BM_Lookup_MediumMap<absl::btree_map<Key, Value>>)->Name("Lookup/MediumMap/AbslBtreeMap");

// =============================================================================
// LOOKUP Benchmarks - Large Map (4096 elements)
// =============================================================================

template<typename Container>
static void BM_Lookup_LargeMap(benchmark::State& state) {
    Container map;
    for (int i = 0; i < LARGE_MAP_SIZE; ++i) {
        map[Key(i)] = Value(i);
    }
    auto keys = make_shuffled_keys(LARGE_MAP_SIZE);
    
    for (auto _ : state) {
        for (Key key : keys) {
            auto it = map.find(key);
            benchmark::DoNotOptimize(it);
        }
    }
}

BENCHMARK(BM_Lookup_LargeMap<std::map<Key, Value>>)->Name("Lookup/LargeMap/StdMap");
BENCHMARK(BM_Lookup_LargeMap<boost::container::flat_map<Key, Value>>)->Name("Lookup/LargeMap/BoostFlatMap");
BENCHMARK(BM_Lookup_LargeMap<absl::btree_map<Key, Value>>)->Name("Lookup/LargeMap/AbslBtreeMap");

BENCHMARK_MAIN();
