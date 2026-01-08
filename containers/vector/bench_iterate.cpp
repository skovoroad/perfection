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
static void BM_Iterate_Small(benchmark::State& state) {
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
// Small + SmallElement (int)
// =============================================================================

BENCHMARK(BM_Iterate_Small<std::vector<SmallElement>, SmallElement>)->Name("Iterate/Small_int/StdVector");
BENCHMARK(BM_Iterate_Small<boost::container::vector<SmallElement>, SmallElement>)->Name("Iterate/Small_int/BoostVector");
BENCHMARK(BM_Iterate_Small<boost::container::small_vector<SmallElement, 8>, SmallElement>)->Name("Iterate/Small_int/SmallVector");
BENCHMARK(BM_Iterate_Small<boost::container::static_vector<SmallElement, 8>, SmallElement>)->Name("Iterate/Small_int/StaticVector");
BENCHMARK(BM_Iterate_Small<absl::InlinedVector<SmallElement, 8>, SmallElement>)->Name("Iterate/Small_int/InlinedVector");

// =============================================================================
// Medium iteration (64 elements)
// =============================================================================

template<typename Container, typename Element>
static void BM_Iterate_Medium(benchmark::State& state) {
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

BENCHMARK(BM_Iterate_Medium<std::vector<SmallElement>, SmallElement>)->Name("Iterate/Medium_int/StdVector");
BENCHMARK(BM_Iterate_Medium<boost::container::vector<SmallElement>, SmallElement>)->Name("Iterate/Medium_int/BoostVector");
BENCHMARK(BM_Iterate_Medium<boost::container::small_vector<SmallElement, 8>, SmallElement>)->Name("Iterate/Medium_int/SmallVector");
BENCHMARK(BM_Iterate_Medium<boost::container::static_vector<SmallElement, 64>, SmallElement>)->Name("Iterate/Medium_int/StaticVector");
BENCHMARK(BM_Iterate_Medium<absl::InlinedVector<SmallElement, 8>, SmallElement>)->Name("Iterate/Medium_int/InlinedVector");

BENCHMARK(BM_Iterate_Medium<std::vector<Point>, Point>)->Name("Iterate/Medium_Point/StdVector");
BENCHMARK(BM_Iterate_Medium<boost::container::vector<Point>, Point>)->Name("Iterate/Medium_Point/BoostVector");
BENCHMARK(BM_Iterate_Medium<boost::container::small_vector<Point, 8>, Point>)->Name("Iterate/Medium_Point/SmallVector");
BENCHMARK(BM_Iterate_Medium<boost::container::static_vector<Point, 64>, Point>)->Name("Iterate/Medium_Point/StaticVector");
BENCHMARK(BM_Iterate_Medium<absl::InlinedVector<Point, 8>, Point>)->Name("Iterate/Medium_Point/InlinedVector");

BENCHMARK(BM_Iterate_Medium<std::vector<LargeStruct>, LargeStruct>)->Name("Iterate/Medium_LargeStruct/StdVector");
BENCHMARK(BM_Iterate_Medium<boost::container::vector<LargeStruct>, LargeStruct>)->Name("Iterate/Medium_LargeStruct/BoostVector");
BENCHMARK(BM_Iterate_Medium<boost::container::small_vector<LargeStruct, 8>, LargeStruct>)->Name("Iterate/Medium_LargeStruct/SmallVector");
BENCHMARK(BM_Iterate_Medium<boost::container::static_vector<LargeStruct, 64>, LargeStruct>)->Name("Iterate/Medium_LargeStruct/StaticVector");
BENCHMARK(BM_Iterate_Medium<absl::InlinedVector<LargeStruct, 8>, LargeStruct>)->Name("Iterate/Medium_LargeStruct/InlinedVector");

// =============================================================================
// Large iteration (1024 elements)
// =============================================================================

template<typename Container, typename Element>
static void BM_Iterate_Large(benchmark::State& state) {
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

BENCHMARK(BM_Iterate_Large<std::vector<SmallElement>, SmallElement>)->Name("Iterate/Large_int/StdVector");
BENCHMARK(BM_Iterate_Large<boost::container::vector<SmallElement>, SmallElement>)->Name("Iterate/Large_int/BoostVector");
BENCHMARK(BM_Iterate_Large<boost::container::small_vector<SmallElement, 8>, SmallElement>)->Name("Iterate/Large_int/SmallVector");
BENCHMARK(BM_Iterate_Large<boost::container::static_vector<SmallElement, 1024>, SmallElement>)->Name("Iterate/Large_int/StaticVector");
BENCHMARK(BM_Iterate_Large<absl::InlinedVector<SmallElement, 8>, SmallElement>)->Name("Iterate/Large_int/InlinedVector");

BENCHMARK(BM_Iterate_Large<std::vector<Point>, Point>)->Name("Iterate/Large_Point/StdVector");
BENCHMARK(BM_Iterate_Large<boost::container::vector<Point>, Point>)->Name("Iterate/Large_Point/BoostVector");
BENCHMARK(BM_Iterate_Large<boost::container::small_vector<Point, 8>, Point>)->Name("Iterate/Large_Point/SmallVector");
BENCHMARK(BM_Iterate_Large<boost::container::static_vector<Point, 1024>, Point>)->Name("Iterate/Large_Point/StaticVector");
BENCHMARK(BM_Iterate_Large<absl::InlinedVector<Point, 8>, Point>)->Name("Iterate/Large_Point/InlinedVector");

BENCHMARK(BM_Iterate_Large<std::vector<LargeStruct>, LargeStruct>)->Name("Iterate/Large_LargeStruct/StdVector");
BENCHMARK(BM_Iterate_Large<boost::container::vector<LargeStruct>, LargeStruct>)->Name("Iterate/Large_LargeStruct/BoostVector");
BENCHMARK(BM_Iterate_Large<boost::container::small_vector<LargeStruct, 8>, LargeStruct>)->Name("Iterate/Large_LargeStruct/SmallVector");
BENCHMARK(BM_Iterate_Large<boost::container::static_vector<LargeStruct, 1024>, LargeStruct>)->Name("Iterate/Large_LargeStruct/StaticVector");
BENCHMARK(BM_Iterate_Large<absl::InlinedVector<LargeStruct, 8>, LargeStruct>)->Name("Iterate/Large_LargeStruct/InlinedVector");

BENCHMARK_MAIN();
