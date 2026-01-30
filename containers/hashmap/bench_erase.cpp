#include <benchmark/benchmark.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <random>

// Abseil container
#include <absl/container/flat_hash_map.h>

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

BENCHMARK(BM_Erase_SmallMap<std::unordered_map<Key, Value>>)->Name("Erase/SmallMap/StdUnorderedMap");
BENCHMARK(BM_Erase_SmallMap<absl::flat_hash_map<Key, Value>>)->Name("Erase/SmallMap/AbslFlatHashMap");

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

BENCHMARK(BM_Erase_MediumMap<std::unordered_map<Key, Value>>)->Name("Erase/MediumMap/StdUnorderedMap");
BENCHMARK(BM_Erase_MediumMap<absl::flat_hash_map<Key, Value>>)->Name("Erase/MediumMap/AbslFlatHashMap");

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

BENCHMARK(BM_Erase_LargeMap<std::unordered_map<Key, Value>>)->Name("Erase/LargeMap/StdUnorderedMap");
BENCHMARK(BM_Erase_LargeMap<absl::flat_hash_map<Key, Value>>)->Name("Erase/LargeMap/AbslFlatHashMap");

BENCHMARK_MAIN();
