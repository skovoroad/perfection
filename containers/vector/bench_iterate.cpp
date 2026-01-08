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
// ITERATE Benchmarks
// =============================================================================

template<typename Container, typename Element>
static void BM_Iterate_SmallVector(benchmark::State& state) {
    Container vec;
    for (int i = 0; i < 8; ++i) {
        vec.push_back(Element(i));
    }
    
    for (auto _ : state) {
        typename Container::value_type sum{};
        for (const auto& elem : vec) {
            benchmark::DoNotOptimize(sum);
            sum = elem;
        }
        benchmark::DoNotOptimize(sum);
    }
}

// =============================================================================
// SmallVector + SmallItem
// =============================================================================

BENCHMARK(BM_Iterate_SmallVector<std::vector<SmallItem>, SmallItem>)->Name("Iterate/SmallVector_SmallItem/StdVector");
BENCHMARK(BM_Iterate_SmallVector<boost::container::vector<SmallItem>, SmallItem>)->Name("Iterate/SmallVector_SmallItem/BoostVector");
BENCHMARK(BM_Iterate_SmallVector<boost::container::small_vector<SmallItem, 8>, SmallItem>)->Name("Iterate/SmallVector_SmallItem/SmallVector");
BENCHMARK(BM_Iterate_SmallVector<boost::container::static_vector<SmallItem, 8>, SmallItem>)->Name("Iterate/SmallVector_SmallItem/StaticVector");
BENCHMARK(BM_Iterate_SmallVector<absl::InlinedVector<SmallItem, 8>, SmallItem>)->Name("Iterate/SmallVector_SmallItem/InlinedVector");

// =============================================================================
// MediumVector iteration (64 elements)
// =============================================================================

template<typename Container, typename Element>
static void BM_Iterate_MediumVector(benchmark::State& state) {
    Container vec;
    for (int i = 0; i < 64; ++i) {
        vec.push_back(Element(i));
    }
    
    for (auto _ : state) {
        typename Container::value_type sum{};
        for (const auto& elem : vec) {
            benchmark::DoNotOptimize(sum);
            sum = elem;
        }
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK(BM_Iterate_MediumVector<std::vector<SmallItem>, SmallItem>)->Name("Iterate/MediumVector_SmallItem/StdVector");
BENCHMARK(BM_Iterate_MediumVector<boost::container::vector<SmallItem>, SmallItem>)->Name("Iterate/MediumVector_SmallItem/BoostVector");
BENCHMARK(BM_Iterate_MediumVector<boost::container::small_vector<SmallItem, 8>, SmallItem>)->Name("Iterate/MediumVector_SmallItem/SmallVector");
BENCHMARK(BM_Iterate_MediumVector<boost::container::static_vector<SmallItem, 64>, SmallItem>)->Name("Iterate/MediumVector_SmallItem/StaticVector");
BENCHMARK(BM_Iterate_MediumVector<absl::InlinedVector<SmallItem, 8>, SmallItem>)->Name("Iterate/MediumVector_SmallItem/InlinedVector");

BENCHMARK(BM_Iterate_MediumVector<std::vector<MediumItem>, MediumItem>)->Name("Iterate/MediumVector_MediumItem/StdVector");
BENCHMARK(BM_Iterate_MediumVector<boost::container::vector<MediumItem>, MediumItem>)->Name("Iterate/MediumVector_MediumItem/BoostVector");
BENCHMARK(BM_Iterate_MediumVector<boost::container::small_vector<MediumItem, 8>, MediumItem>)->Name("Iterate/MediumVector_MediumItem/SmallVector");
BENCHMARK(BM_Iterate_MediumVector<boost::container::static_vector<MediumItem, 64>, MediumItem>)->Name("Iterate/MediumVector_MediumItem/StaticVector");
BENCHMARK(BM_Iterate_MediumVector<absl::InlinedVector<MediumItem, 8>, MediumItem>)->Name("Iterate/MediumVector_MediumItem/InlinedVector");

BENCHMARK(BM_Iterate_MediumVector<std::vector<LargeItem>, LargeItem>)->Name("Iterate/MediumVector_LargeItem/StdVector");
BENCHMARK(BM_Iterate_MediumVector<boost::container::vector<LargeItem>, LargeItem>)->Name("Iterate/MediumVector_LargeItem/BoostVector");
BENCHMARK(BM_Iterate_MediumVector<boost::container::small_vector<LargeItem, 8>, LargeItem>)->Name("Iterate/MediumVector_LargeItem/SmallVector");
BENCHMARK(BM_Iterate_MediumVector<boost::container::static_vector<LargeItem, 64>, LargeItem>)->Name("Iterate/MediumVector_LargeItem/StaticVector");
BENCHMARK(BM_Iterate_MediumVector<absl::InlinedVector<LargeItem, 8>, LargeItem>)->Name("Iterate/MediumVector_LargeItem/InlinedVector");

// =============================================================================
// LargeVector iteration (1024 elements)
// =============================================================================

template<typename Container, typename Element>
static void BM_Iterate_LargeVector(benchmark::State& state) {
    Container vec;
    for (int i = 0; i < 1024; ++i) {
        vec.push_back(Element(i));
    }
    
    for (auto _ : state) {
        typename Container::value_type sum{};
        for (const auto& elem : vec) {
            benchmark::DoNotOptimize(sum);
            sum = elem;
        }
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK(BM_Iterate_LargeVector<std::vector<SmallItem>, SmallItem>)->Name("Iterate/LargeVector_SmallItem/StdVector");
BENCHMARK(BM_Iterate_LargeVector<boost::container::vector<SmallItem>, SmallItem>)->Name("Iterate/LargeVector_SmallItem/BoostVector");
BENCHMARK(BM_Iterate_LargeVector<boost::container::small_vector<SmallItem, 8>, SmallItem>)->Name("Iterate/LargeVector_SmallItem/SmallVector");
BENCHMARK(BM_Iterate_LargeVector<boost::container::static_vector<SmallItem, 1024>, SmallItem>)->Name("Iterate/LargeVector_SmallItem/StaticVector");
BENCHMARK(BM_Iterate_LargeVector<absl::InlinedVector<SmallItem, 8>, SmallItem>)->Name("Iterate/LargeVector_SmallItem/InlinedVector");

BENCHMARK(BM_Iterate_LargeVector<std::vector<MediumItem>, MediumItem>)->Name("Iterate/LargeVector_MediumItem/StdVector");
BENCHMARK(BM_Iterate_LargeVector<boost::container::vector<MediumItem>, MediumItem>)->Name("Iterate/LargeVector_MediumItem/BoostVector");
BENCHMARK(BM_Iterate_LargeVector<boost::container::small_vector<MediumItem, 8>, MediumItem>)->Name("Iterate/LargeVector_MediumItem/SmallVector");
BENCHMARK(BM_Iterate_LargeVector<boost::container::static_vector<MediumItem, 1024>, MediumItem>)->Name("Iterate/LargeVector_MediumItem/StaticVector");
BENCHMARK(BM_Iterate_LargeVector<absl::InlinedVector<MediumItem, 8>, MediumItem>)->Name("Iterate/LargeVector_MediumItem/InlinedVector");

BENCHMARK(BM_Iterate_LargeVector<std::vector<LargeItem>, LargeItem>)->Name("Iterate/LargeVector_LargeItem/StdVector");
BENCHMARK(BM_Iterate_LargeVector<boost::container::vector<LargeItem>, LargeItem>)->Name("Iterate/LargeVector_LargeItem/BoostVector");
BENCHMARK(BM_Iterate_LargeVector<boost::container::small_vector<LargeItem, 8>, LargeItem>)->Name("Iterate/LargeVector_LargeItem/SmallVector");
BENCHMARK(BM_Iterate_LargeVector<boost::container::static_vector<LargeItem, 1024>, LargeItem>)->Name("Iterate/LargeVector_LargeItem/StaticVector");
BENCHMARK(BM_Iterate_LargeVector<absl::InlinedVector<LargeItem, 8>, LargeItem>)->Name("Iterate/LargeVector_LargeItem/InlinedVector");

BENCHMARK_MAIN();
