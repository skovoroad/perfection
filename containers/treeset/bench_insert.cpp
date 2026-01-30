#include <benchmark/benchmark.h>
#include <set>

// Boost containers
#include <boost/container/flat_set.hpp>

// Abseil container
#include <absl/container/btree_set.h>

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

BENCHMARK(BM_Insert_SmallSet<std::set<Element>>)->Name("Insert/SmallSet/StdSet");
BENCHMARK(BM_Insert_SmallSet<boost::container::flat_set<Element>>)->Name("Insert/SmallSet/BoostFlatSet");
BENCHMARK(BM_Insert_SmallSet<absl::btree_set<Element>>)->Name("Insert/SmallSet/AbslBtreeSet");

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

BENCHMARK(BM_Insert_MediumSet<std::set<Element>>)->Name("Insert/MediumSet/StdSet");
BENCHMARK(BM_Insert_MediumSet<boost::container::flat_set<Element>>)->Name("Insert/MediumSet/BoostFlatSet");
BENCHMARK(BM_Insert_MediumSet<absl::btree_set<Element>>)->Name("Insert/MediumSet/AbslBtreeSet");

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

BENCHMARK(BM_Insert_LargeSet<std::set<Element>>)->Name("Insert/LargeSet/StdSet");
BENCHMARK(BM_Insert_LargeSet<boost::container::flat_set<Element>>)->Name("Insert/LargeSet/BoostFlatSet");
BENCHMARK(BM_Insert_LargeSet<absl::btree_set<Element>>)->Name("Insert/LargeSet/AbslBtreeSet");

BENCHMARK_MAIN();
