#include <benchmark/benchmark.h>
#include <unordered_set>

// Abseil container
#include <absl/container/flat_hash_set.h>

#include "common.h"

// =============================================================================
// INSERT Benchmarks - Small Set (16 elements)
// =============================================================================

template<typename Container>
static void BM_Insert_SmallSet(benchmark::State& state) {
    for (auto _ : state) {
        Container set;
        for (int i = 0; i < SMALL_SET_SIZE; ++i) {
            set.insert(Element(i));
        }
        benchmark::DoNotOptimize(set);
        benchmark::ClobberMemory();  // Prevent caching set state between iterations
    }
}

template<typename Container>
static void BM_Insert_SmallSet_Reserved(benchmark::State& state) {
    for (auto _ : state) {
        Container set;
        set.reserve(SMALL_SET_SIZE);
        for (int i = 0; i < SMALL_SET_SIZE; ++i) {
            set.insert(Element(i));
        }
        benchmark::DoNotOptimize(set);
        benchmark::ClobberMemory();  // Prevent caching set state between iterations
    }
}

BENCHMARK(BM_Insert_SmallSet<std::unordered_set<Element>>)->Name("Insert/SmallSet/StdUnorderedSet");
BENCHMARK(BM_Insert_SmallSet<absl::flat_hash_set<Element>>)->Name("Insert/SmallSet/AbslFlatHashSet");
BENCHMARK(BM_Insert_SmallSet_Reserved<std::unordered_set<Element>>)->Name("Insert/SmallSet/StdUnorderedSet_Reserved");
BENCHMARK(BM_Insert_SmallSet_Reserved<absl::flat_hash_set<Element>>)->Name("Insert/SmallSet/AbslFlatHashSet_Reserved");

// =============================================================================
// INSERT Benchmarks - Medium Set (256 elements)
// =============================================================================

template<typename Container>
static void BM_Insert_MediumSet(benchmark::State& state) {
    for (auto _ : state) {
        Container set;
        for (int i = 0; i < MEDIUM_SET_SIZE; ++i) {
            set.insert(Element(i));
        }
        benchmark::DoNotOptimize(set);
        benchmark::ClobberMemory();  // Prevent caching set state between iterations
    }
}

template<typename Container>
static void BM_Insert_MediumSet_Reserved(benchmark::State& state) {
    for (auto _ : state) {
        Container set;
        set.reserve(MEDIUM_SET_SIZE);
        for (int i = 0; i < MEDIUM_SET_SIZE; ++i) {
            set.insert(Element(i));
        }
        benchmark::DoNotOptimize(set);
        benchmark::ClobberMemory();  // Prevent caching set state between iterations
    }
}

BENCHMARK(BM_Insert_MediumSet<std::unordered_set<Element>>)->Name("Insert/MediumSet/StdUnorderedSet");
BENCHMARK(BM_Insert_MediumSet<absl::flat_hash_set<Element>>)->Name("Insert/MediumSet/AbslFlatHashSet");
BENCHMARK(BM_Insert_MediumSet_Reserved<std::unordered_set<Element>>)->Name("Insert/MediumSet/StdUnorderedSet_Reserved");
BENCHMARK(BM_Insert_MediumSet_Reserved<absl::flat_hash_set<Element>>)->Name("Insert/MediumSet/AbslFlatHashSet_Reserved");

// =============================================================================
// INSERT Benchmarks - Large Set (4096 elements)
// =============================================================================

template<typename Container>
static void BM_Insert_LargeSet(benchmark::State& state) {
    for (auto _ : state) {
        Container set;
        for (int i = 0; i < LARGE_SET_SIZE; ++i) {
            set.insert(Element(i));
        }
        benchmark::DoNotOptimize(set);
        benchmark::ClobberMemory();  // Prevent caching set state between iterations
    }
}

template<typename Container>
static void BM_Insert_LargeSet_Reserved(benchmark::State& state) {
    for (auto _ : state) {
        Container set;
        set.reserve(LARGE_SET_SIZE);
        for (int i = 0; i < LARGE_SET_SIZE; ++i) {
            set.insert(Element(i));
        }
        benchmark::DoNotOptimize(set);
        benchmark::ClobberMemory();  // Prevent caching set state between iterations
    }
}

BENCHMARK(BM_Insert_LargeSet<std::unordered_set<Element>>)->Name("Insert/LargeSet/StdUnorderedSet");
BENCHMARK(BM_Insert_LargeSet<absl::flat_hash_set<Element>>)->Name("Insert/LargeSet/AbslFlatHashSet");
BENCHMARK(BM_Insert_LargeSet_Reserved<std::unordered_set<Element>>)->Name("Insert/LargeSet/StdUnorderedSet_Reserved");
BENCHMARK(BM_Insert_LargeSet_Reserved<absl::flat_hash_set<Element>>)->Name("Insert/LargeSet/AbslFlatHashSet_Reserved");

BENCHMARK_MAIN();
