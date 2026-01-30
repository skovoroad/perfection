#include <benchmark/benchmark.h>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <random>

// Abseil container
#include <absl/container/flat_hash_set.h>

#include "common.h"

// =============================================================================
// Helper: create shuffled elements for random erase order
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
// ERASE Benchmarks - Small Set (16 elements)
// =============================================================================

template<typename Container>
static void BM_Erase_SmallSet(benchmark::State& state) {
    auto elements = make_shuffled_elements(SMALL_SET_SIZE);
    
    for (auto _ : state) {
        state.PauseTiming();
        Container set;
        for (int i = 0; i < SMALL_SET_SIZE; ++i) {
            set.insert(Element(i));
        }
        state.ResumeTiming();
        
        for (Element elem : elements) {
            set.erase(elem);
        }
        benchmark::DoNotOptimize(set);
    }
}

BENCHMARK(BM_Erase_SmallSet<std::unordered_set<Element>>)->Name("Erase/SmallSet/StdUnorderedSet");
BENCHMARK(BM_Erase_SmallSet<absl::flat_hash_set<Element>>)->Name("Erase/SmallSet/AbslFlatHashSet");

// =============================================================================
// ERASE Benchmarks - Medium Set (256 elements)
// =============================================================================

template<typename Container>
static void BM_Erase_MediumSet(benchmark::State& state) {
    auto elements = make_shuffled_elements(MEDIUM_SET_SIZE);
    
    for (auto _ : state) {
        state.PauseTiming();
        Container set;
        for (int i = 0; i < MEDIUM_SET_SIZE; ++i) {
            set.insert(Element(i));
        }
        state.ResumeTiming();
        
        for (Element elem : elements) {
            set.erase(elem);
        }
        benchmark::DoNotOptimize(set);
    }
}

BENCHMARK(BM_Erase_MediumSet<std::unordered_set<Element>>)->Name("Erase/MediumSet/StdUnorderedSet");
BENCHMARK(BM_Erase_MediumSet<absl::flat_hash_set<Element>>)->Name("Erase/MediumSet/AbslFlatHashSet");

// =============================================================================
// ERASE Benchmarks - Large Set (4096 elements)
// =============================================================================

template<typename Container>
static void BM_Erase_LargeSet(benchmark::State& state) {
    auto elements = make_shuffled_elements(LARGE_SET_SIZE);
    
    for (auto _ : state) {
        state.PauseTiming();
        Container set;
        for (int i = 0; i < LARGE_SET_SIZE; ++i) {
            set.insert(Element(i));
        }
        state.ResumeTiming();
        
        for (Element elem : elements) {
            set.erase(elem);
        }
        benchmark::DoNotOptimize(set);
    }
}

BENCHMARK(BM_Erase_LargeSet<std::unordered_set<Element>>)->Name("Erase/LargeSet/StdUnorderedSet");
BENCHMARK(BM_Erase_LargeSet<absl::flat_hash_set<Element>>)->Name("Erase/LargeSet/AbslFlatHashSet");

BENCHMARK_MAIN();
