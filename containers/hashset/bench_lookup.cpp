#include <benchmark/benchmark.h>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <random>

// Abseil container
#include <absl/container/flat_hash_set.h>

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

BENCHMARK(BM_Lookup_SmallSet<std::unordered_set<Element>>)->Name("Lookup/SmallSet/StdUnorderedSet");
BENCHMARK(BM_Lookup_SmallSet<absl::flat_hash_set<Element>>)->Name("Lookup/SmallSet/AbslFlatHashSet");

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

BENCHMARK(BM_Lookup_MediumSet<std::unordered_set<Element>>)->Name("Lookup/MediumSet/StdUnorderedSet");
BENCHMARK(BM_Lookup_MediumSet<absl::flat_hash_set<Element>>)->Name("Lookup/MediumSet/AbslFlatHashSet");

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

BENCHMARK(BM_Lookup_LargeSet<std::unordered_set<Element>>)->Name("Lookup/LargeSet/StdUnorderedSet");
BENCHMARK(BM_Lookup_LargeSet<absl::flat_hash_set<Element>>)->Name("Lookup/LargeSet/AbslFlatHashSet");

BENCHMARK_MAIN();
