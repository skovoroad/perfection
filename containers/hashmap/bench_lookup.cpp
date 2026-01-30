#include <benchmark/benchmark.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <random>

// Abseil container
#include <absl/container/flat_hash_map.h>

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

BENCHMARK(BM_Lookup_SmallMap<std::unordered_map<Key, Value>>)->Name("Lookup/SmallMap/StdUnorderedMap");
BENCHMARK(BM_Lookup_SmallMap<absl::flat_hash_map<Key, Value>>)->Name("Lookup/SmallMap/AbslFlatHashMap");

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

BENCHMARK(BM_Lookup_MediumMap<std::unordered_map<Key, Value>>)->Name("Lookup/MediumMap/StdUnorderedMap");
BENCHMARK(BM_Lookup_MediumMap<absl::flat_hash_map<Key, Value>>)->Name("Lookup/MediumMap/AbslFlatHashMap");

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

BENCHMARK(BM_Lookup_LargeMap<std::unordered_map<Key, Value>>)->Name("Lookup/LargeMap/StdUnorderedMap");
BENCHMARK(BM_Lookup_LargeMap<absl::flat_hash_map<Key, Value>>)->Name("Lookup/LargeMap/AbslFlatHashMap");

BENCHMARK_MAIN();
