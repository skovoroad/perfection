#include <benchmark/benchmark.h>
#include <set>
#include <vector>
#include <algorithm>
#include <random>

// Boost containers
#include <boost/container/flat_set.hpp>

// Abseil container
#include <absl/container/btree_set.h>

#include "common.h"

// =============================================================================
// Helper: create and shuffle elements for random access
// =============================================================================

static std::vector<Element> make_shuffled_elements(int count) {
    std::vector<Element> elements(count);
    for (int i = 0; i < count; ++i) {
        elements[i] = Element(i);
    }
    std::shuffle(elements.begin(), elements.end(), std::mt19937{42});
    return elements;
}

// =============================================================================
// LOOKUP Benchmarks - Small Set (16 elements)
// =============================================================================

template<typename Container>
static void BM_Lookup_SmallSet(benchmark::State& state) {
    Container set;
    for (int i = 0; i < SMALL_SET_SIZE; ++i) {
        set.insert(Element(i));
    }
    auto elements = make_shuffled_elements(SMALL_SET_SIZE);
    
    for (auto _ : state) {
        for (Element elem : elements) {
            auto it = set.find(elem);
            benchmark::DoNotOptimize(it);
        }
    }
}

BENCHMARK(BM_Lookup_SmallSet<std::set<Element>>)->Name("Lookup/SmallSet/StdSet");
BENCHMARK(BM_Lookup_SmallSet<boost::container::flat_set<Element>>)->Name("Lookup/SmallSet/BoostFlatSet");
BENCHMARK(BM_Lookup_SmallSet<absl::btree_set<Element>>)->Name("Lookup/SmallSet/AbslBtreeSet");

// =============================================================================
// LOOKUP Benchmarks - Medium Set (256 elements)
// =============================================================================

template<typename Container>
static void BM_Lookup_MediumSet(benchmark::State& state) {
    Container set;
    for (int i = 0; i < MEDIUM_SET_SIZE; ++i) {
        set.insert(Element(i));
    }
    auto elements = make_shuffled_elements(MEDIUM_SET_SIZE);
    
    for (auto _ : state) {
        for (Element elem : elements) {
            auto it = set.find(elem);
            benchmark::DoNotOptimize(it);
        }
    }
}

BENCHMARK(BM_Lookup_MediumSet<std::set<Element>>)->Name("Lookup/MediumSet/StdSet");
BENCHMARK(BM_Lookup_MediumSet<boost::container::flat_set<Element>>)->Name("Lookup/MediumSet/BoostFlatSet");
BENCHMARK(BM_Lookup_MediumSet<absl::btree_set<Element>>)->Name("Lookup/MediumSet/AbslBtreeSet");

// =============================================================================
// LOOKUP Benchmarks - Large Set (4096 elements)
// =============================================================================

template<typename Container>
static void BM_Lookup_LargeSet(benchmark::State& state) {
    Container set;
    for (int i = 0; i < LARGE_SET_SIZE; ++i) {
        set.insert(Element(i));
    }
    auto elements = make_shuffled_elements(LARGE_SET_SIZE);
    
    for (auto _ : state) {
        for (Element elem : elements) {
            auto it = set.find(elem);
            benchmark::DoNotOptimize(it);
        }
    }
}

BENCHMARK(BM_Lookup_LargeSet<std::set<Element>>)->Name("Lookup/LargeSet/StdSet");
BENCHMARK(BM_Lookup_LargeSet<boost::container::flat_set<Element>>)->Name("Lookup/LargeSet/BoostFlatSet");
BENCHMARK(BM_Lookup_LargeSet<absl::btree_set<Element>>)->Name("Lookup/LargeSet/AbslBtreeSet");

BENCHMARK_MAIN();
