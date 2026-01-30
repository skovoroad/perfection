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

BENCHMARK(BM_Erase_SmallSet<std::set<Element>>)->Name("Erase/SmallSet/StdSet");
BENCHMARK(BM_Erase_SmallSet<boost::container::flat_set<Element>>)->Name("Erase/SmallSet/BoostFlatSet");
BENCHMARK(BM_Erase_SmallSet<absl::btree_set<Element>>)->Name("Erase/SmallSet/AbslBtreeSet");

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

BENCHMARK(BM_Erase_MediumSet<std::set<Element>>)->Name("Erase/MediumSet/StdSet");
BENCHMARK(BM_Erase_MediumSet<boost::container::flat_set<Element>>)->Name("Erase/MediumSet/BoostFlatSet");
BENCHMARK(BM_Erase_MediumSet<absl::btree_set<Element>>)->Name("Erase/MediumSet/AbslBtreeSet");

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

BENCHMARK(BM_Erase_LargeSet<std::set<Element>>)->Name("Erase/LargeSet/StdSet");
BENCHMARK(BM_Erase_LargeSet<boost::container::flat_set<Element>>)->Name("Erase/LargeSet/BoostFlatSet");
BENCHMARK(BM_Erase_LargeSet<absl::btree_set<Element>>)->Name("Erase/LargeSet/AbslBtreeSet");

BENCHMARK_MAIN();
