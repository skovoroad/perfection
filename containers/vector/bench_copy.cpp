#include <benchmark/benchmark.h>
#include <vector>

// Boost containers
#include <boost/container/vector.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/container/static_vector.hpp>

// Abseil container
#include <absl/container/inlined_vector.h>

#include "common.h"

// =============================================================================
// COPY Benchmarks
// =============================================================================

template<typename Container, typename Element>
static void BM_Copy_SmallVector(benchmark::State& state) {
    Container original;
    for (int i = 0; i < 8; ++i) {
        original.push_back(Element(i));
    }
    
    for (auto _ : state) {
        Container copy = original;
        benchmark::DoNotOptimize(copy.data());
        benchmark::ClobberMemory();
    }
}

// =============================================================================
// SmallVector + SmallItem
// =============================================================================

BENCHMARK(BM_Copy_SmallVector<std::vector<SmallItem>, SmallItem>)->Name("Copy/SmallVector_SmallItem/StdVector");
BENCHMARK(BM_Copy_SmallVector<boost::container::vector<SmallItem>, SmallItem>)->Name("Copy/SmallVector_SmallItem/BoostVector");
BENCHMARK(BM_Copy_SmallVector<boost::container::small_vector<SmallItem, 8>, SmallItem>)->Name("Copy/SmallVector_SmallItem/SmallVector");
BENCHMARK(BM_Copy_SmallVector<boost::container::static_vector<SmallItem, 8>, SmallItem>)->Name("Copy/SmallVector_SmallItem/StaticVector");
BENCHMARK(BM_Copy_SmallVector<absl::InlinedVector<SmallItem, 8>, SmallItem>)->Name("Copy/SmallVector_SmallItem/InlinedVector");

// =============================================================================
// SmallVector + MediumItem
// =============================================================================

BENCHMARK(BM_Copy_SmallVector<std::vector<MediumItem>, MediumItem>)->Name("Copy/SmallVector_MediumItem/StdVector");
BENCHMARK(BM_Copy_SmallVector<boost::container::vector<MediumItem>, MediumItem>)->Name("Copy/SmallVector_MediumItem/BoostVector");
BENCHMARK(BM_Copy_SmallVector<boost::container::small_vector<MediumItem, 8>, MediumItem>)->Name("Copy/SmallVector_MediumItem/SmallVector");
BENCHMARK(BM_Copy_SmallVector<boost::container::static_vector<MediumItem, 8>, MediumItem>)->Name("Copy/SmallVector_MediumItem/StaticVector");
BENCHMARK(BM_Copy_SmallVector<absl::InlinedVector<MediumItem, 8>, MediumItem>)->Name("Copy/SmallVector_MediumItem/InlinedVector");

// =============================================================================
// SmallVector + LargeItem
// =============================================================================

BENCHMARK(BM_Copy_SmallVector<std::vector<LargeItem>, LargeItem>)->Name("Copy/SmallVector_LargeItem/StdVector");
BENCHMARK(BM_Copy_SmallVector<boost::container::vector<LargeItem>, LargeItem>)->Name("Copy/SmallVector_LargeItem/BoostVector");
BENCHMARK(BM_Copy_SmallVector<boost::container::small_vector<LargeItem, 8>, LargeItem>)->Name("Copy/SmallVector_LargeItem/SmallVector");
BENCHMARK(BM_Copy_SmallVector<boost::container::static_vector<LargeItem, 8>, LargeItem>)->Name("Copy/SmallVector_LargeItem/StaticVector");
BENCHMARK(BM_Copy_SmallVector<absl::InlinedVector<LargeItem, 8>, LargeItem>)->Name("Copy/SmallVector_LargeItem/InlinedVector");

// =============================================================================
// MediumVector copy (64 elements)
// =============================================================================

template<typename Container, typename Element>
static void BM_Copy_MediumVector(benchmark::State& state) {
    Container original;
    for (int i = 0; i < 64; ++i) {
        original.push_back(Element(i));
    }
    
    for (auto _ : state) {
        Container copy = original;
        benchmark::DoNotOptimize(copy.data());
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_Copy_MediumVector<std::vector<SmallItem>, SmallItem>)->Name("Copy/MediumVector_SmallItem/StdVector");
BENCHMARK(BM_Copy_MediumVector<boost::container::vector<SmallItem>, SmallItem>)->Name("Copy/MediumVector_SmallItem/BoostVector");
BENCHMARK(BM_Copy_MediumVector<boost::container::small_vector<SmallItem, 8>, SmallItem>)->Name("Copy/MediumVector_SmallItem/SmallVector");
BENCHMARK(BM_Copy_MediumVector<boost::container::static_vector<SmallItem, 64>, SmallItem>)->Name("Copy/MediumVector_SmallItem/StaticVector");
BENCHMARK(BM_Copy_MediumVector<absl::InlinedVector<SmallItem, 8>, SmallItem>)->Name("Copy/MediumVector_SmallItem/InlinedVector");

BENCHMARK(BM_Copy_MediumVector<std::vector<MediumItem>, MediumItem>)->Name("Copy/MediumVector_MediumItem/StdVector");
BENCHMARK(BM_Copy_MediumVector<boost::container::vector<MediumItem>, MediumItem>)->Name("Copy/MediumVector_MediumItem/BoostVector");
BENCHMARK(BM_Copy_MediumVector<boost::container::small_vector<MediumItem, 8>, MediumItem>)->Name("Copy/MediumVector_MediumItem/SmallVector");
BENCHMARK(BM_Copy_MediumVector<boost::container::static_vector<MediumItem, 64>, MediumItem>)->Name("Copy/MediumVector_MediumItem/StaticVector");
BENCHMARK(BM_Copy_MediumVector<absl::InlinedVector<MediumItem, 8>, MediumItem>)->Name("Copy/MediumVector_MediumItem/InlinedVector");

BENCHMARK(BM_Copy_MediumVector<std::vector<LargeItem>, LargeItem>)->Name("Copy/MediumVector_LargeItem/StdVector");
BENCHMARK(BM_Copy_MediumVector<boost::container::vector<LargeItem>, LargeItem>)->Name("Copy/MediumVector_LargeItem/BoostVector");
BENCHMARK(BM_Copy_MediumVector<boost::container::small_vector<LargeItem, 8>, LargeItem>)->Name("Copy/MediumVector_LargeItem/SmallVector");
BENCHMARK(BM_Copy_MediumVector<boost::container::static_vector<LargeItem, 64>, LargeItem>)->Name("Copy/MediumVector_LargeItem/StaticVector");
BENCHMARK(BM_Copy_MediumVector<absl::InlinedVector<LargeItem, 8>, LargeItem>)->Name("Copy/MediumVector_LargeItem/InlinedVector");

// =============================================================================
// LargeVector copy (1024 elements)
// =============================================================================

template<typename Container, typename Element>
static void BM_Copy_LargeVector(benchmark::State& state) {
    Container original;
    for (int i = 0; i < 1024; ++i) {
        original.push_back(Element(i));
    }
    
    for (auto _ : state) {
        Container copy = original;
        benchmark::DoNotOptimize(copy.data());
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_Copy_LargeVector<std::vector<SmallItem>, SmallItem>)->Name("Copy/LargeVector_SmallItem/StdVector");
BENCHMARK(BM_Copy_LargeVector<boost::container::vector<SmallItem>, SmallItem>)->Name("Copy/LargeVector_SmallItem/BoostVector");
BENCHMARK(BM_Copy_LargeVector<boost::container::small_vector<SmallItem, 8>, SmallItem>)->Name("Copy/LargeVector_SmallItem/SmallVector");
BENCHMARK(BM_Copy_LargeVector<boost::container::static_vector<SmallItem, 1024>, SmallItem>)->Name("Copy/LargeVector_SmallItem/StaticVector");
BENCHMARK(BM_Copy_LargeVector<absl::InlinedVector<SmallItem, 8>, SmallItem>)->Name("Copy/LargeVector_SmallItem/InlinedVector");

BENCHMARK(BM_Copy_LargeVector<std::vector<MediumItem>, MediumItem>)->Name("Copy/LargeVector_MediumItem/StdVector");
BENCHMARK(BM_Copy_LargeVector<boost::container::vector<MediumItem>, MediumItem>)->Name("Copy/LargeVector_MediumItem/BoostVector");
BENCHMARK(BM_Copy_LargeVector<boost::container::small_vector<MediumItem, 8>, MediumItem>)->Name("Copy/LargeVector_MediumItem/SmallVector");
BENCHMARK(BM_Copy_LargeVector<boost::container::static_vector<MediumItem, 1024>, MediumItem>)->Name("Copy/LargeVector_MediumItem/StaticVector");
BENCHMARK(BM_Copy_LargeVector<absl::InlinedVector<MediumItem, 8>, MediumItem>)->Name("Copy/LargeVector_MediumItem/InlinedVector");

BENCHMARK(BM_Copy_LargeVector<std::vector<LargeItem>, LargeItem>)->Name("Copy/LargeVector_LargeItem/StdVector");
BENCHMARK(BM_Copy_LargeVector<boost::container::vector<LargeItem>, LargeItem>)->Name("Copy/LargeVector_LargeItem/BoostVector");
BENCHMARK(BM_Copy_LargeVector<boost::container::small_vector<LargeItem, 8>, LargeItem>)->Name("Copy/LargeVector_LargeItem/SmallVector");
BENCHMARK(BM_Copy_LargeVector<boost::container::static_vector<LargeItem, 1024>, LargeItem>)->Name("Copy/LargeVector_LargeItem/StaticVector");
BENCHMARK(BM_Copy_LargeVector<absl::InlinedVector<LargeItem, 8>, LargeItem>)->Name("Copy/LargeVector_LargeItem/InlinedVector");

BENCHMARK_MAIN();
