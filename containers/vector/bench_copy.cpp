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
static void BM_Copy_Small(benchmark::State& state) {
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
// Small + SmallElement (int)
// =============================================================================

BENCHMARK(BM_Copy_Small<std::vector<SmallElement>, SmallElement>)->Name("Copy/Small_int/StdVector");
BENCHMARK(BM_Copy_Small<boost::container::vector<SmallElement>, SmallElement>)->Name("Copy/Small_int/BoostVector");
BENCHMARK(BM_Copy_Small<boost::container::small_vector<SmallElement, 8>, SmallElement>)->Name("Copy/Small_int/SmallVector");
BENCHMARK(BM_Copy_Small<boost::container::static_vector<SmallElement, 8>, SmallElement>)->Name("Copy/Small_int/StaticVector");
BENCHMARK(BM_Copy_Small<absl::InlinedVector<SmallElement, 8>, SmallElement>)->Name("Copy/Small_int/InlinedVector");

// =============================================================================
// Small + Point
// =============================================================================

BENCHMARK(BM_Copy_Small<std::vector<Point>, Point>)->Name("Copy/Small_Point/StdVector");
BENCHMARK(BM_Copy_Small<boost::container::vector<Point>, Point>)->Name("Copy/Small_Point/BoostVector");
BENCHMARK(BM_Copy_Small<boost::container::small_vector<Point, 8>, Point>)->Name("Copy/Small_Point/SmallVector");
BENCHMARK(BM_Copy_Small<boost::container::static_vector<Point, 8>, Point>)->Name("Copy/Small_Point/StaticVector");
BENCHMARK(BM_Copy_Small<absl::InlinedVector<Point, 8>, Point>)->Name("Copy/Small_Point/InlinedVector");

// =============================================================================
// Small + LargeStruct
// =============================================================================

BENCHMARK(BM_Copy_Small<std::vector<LargeStruct>, LargeStruct>)->Name("Copy/Small_LargeStruct/StdVector");
BENCHMARK(BM_Copy_Small<boost::container::vector<LargeStruct>, LargeStruct>)->Name("Copy/Small_LargeStruct/BoostVector");
BENCHMARK(BM_Copy_Small<boost::container::small_vector<LargeStruct, 8>, LargeStruct>)->Name("Copy/Small_LargeStruct/SmallVector");
BENCHMARK(BM_Copy_Small<boost::container::static_vector<LargeStruct, 8>, LargeStruct>)->Name("Copy/Small_LargeStruct/StaticVector");
BENCHMARK(BM_Copy_Small<absl::InlinedVector<LargeStruct, 8>, LargeStruct>)->Name("Copy/Small_LargeStruct/InlinedVector");

// =============================================================================
// Medium copy (64 elements)
// =============================================================================

template<typename Container, typename Element>
static void BM_Copy_Medium(benchmark::State& state) {
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

BENCHMARK(BM_Copy_Medium<std::vector<SmallElement>, SmallElement>)->Name("Copy/Medium_int/StdVector");
BENCHMARK(BM_Copy_Medium<boost::container::vector<SmallElement>, SmallElement>)->Name("Copy/Medium_int/BoostVector");
BENCHMARK(BM_Copy_Medium<boost::container::small_vector<SmallElement, 8>, SmallElement>)->Name("Copy/Medium_int/SmallVector");
BENCHMARK(BM_Copy_Medium<boost::container::static_vector<SmallElement, 64>, SmallElement>)->Name("Copy/Medium_int/StaticVector");
BENCHMARK(BM_Copy_Medium<absl::InlinedVector<SmallElement, 8>, SmallElement>)->Name("Copy/Medium_int/InlinedVector");

BENCHMARK(BM_Copy_Medium<std::vector<Point>, Point>)->Name("Copy/Medium_Point/StdVector");
BENCHMARK(BM_Copy_Medium<boost::container::vector<Point>, Point>)->Name("Copy/Medium_Point/BoostVector");
BENCHMARK(BM_Copy_Medium<boost::container::small_vector<Point, 8>, Point>)->Name("Copy/Medium_Point/SmallVector");
BENCHMARK(BM_Copy_Medium<boost::container::static_vector<Point, 64>, Point>)->Name("Copy/Medium_Point/StaticVector");
BENCHMARK(BM_Copy_Medium<absl::InlinedVector<Point, 8>, Point>)->Name("Copy/Medium_Point/InlinedVector");

BENCHMARK(BM_Copy_Medium<std::vector<LargeStruct>, LargeStruct>)->Name("Copy/Medium_LargeStruct/StdVector");
BENCHMARK(BM_Copy_Medium<boost::container::vector<LargeStruct>, LargeStruct>)->Name("Copy/Medium_LargeStruct/BoostVector");
BENCHMARK(BM_Copy_Medium<boost::container::small_vector<LargeStruct, 8>, LargeStruct>)->Name("Copy/Medium_LargeStruct/SmallVector");
BENCHMARK(BM_Copy_Medium<boost::container::static_vector<LargeStruct, 64>, LargeStruct>)->Name("Copy/Medium_LargeStruct/StaticVector");
BENCHMARK(BM_Copy_Medium<absl::InlinedVector<LargeStruct, 8>, LargeStruct>)->Name("Copy/Medium_LargeStruct/InlinedVector");

// =============================================================================
// Large copy (1024 elements)
// =============================================================================

template<typename Container, typename Element>
static void BM_Copy_Large(benchmark::State& state) {
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

BENCHMARK(BM_Copy_Large<std::vector<SmallElement>, SmallElement>)->Name("Copy/Large_int/StdVector");
BENCHMARK(BM_Copy_Large<boost::container::vector<SmallElement>, SmallElement>)->Name("Copy/Large_int/BoostVector");
BENCHMARK(BM_Copy_Large<boost::container::small_vector<SmallElement, 8>, SmallElement>)->Name("Copy/Large_int/SmallVector");
BENCHMARK(BM_Copy_Large<boost::container::static_vector<SmallElement, 1024>, SmallElement>)->Name("Copy/Large_int/StaticVector");
BENCHMARK(BM_Copy_Large<absl::InlinedVector<SmallElement, 8>, SmallElement>)->Name("Copy/Large_int/InlinedVector");

BENCHMARK(BM_Copy_Large<std::vector<Point>, Point>)->Name("Copy/Large_Point/StdVector");
BENCHMARK(BM_Copy_Large<boost::container::vector<Point>, Point>)->Name("Copy/Large_Point/BoostVector");
BENCHMARK(BM_Copy_Large<boost::container::small_vector<Point, 8>, Point>)->Name("Copy/Large_Point/SmallVector");
BENCHMARK(BM_Copy_Large<boost::container::static_vector<Point, 1024>, Point>)->Name("Copy/Large_Point/StaticVector");
BENCHMARK(BM_Copy_Large<absl::InlinedVector<Point, 8>, Point>)->Name("Copy/Large_Point/InlinedVector");

BENCHMARK(BM_Copy_Large<std::vector<LargeStruct>, LargeStruct>)->Name("Copy/Large_LargeStruct/StdVector");
BENCHMARK(BM_Copy_Large<boost::container::vector<LargeStruct>, LargeStruct>)->Name("Copy/Large_LargeStruct/BoostVector");
BENCHMARK(BM_Copy_Large<boost::container::small_vector<LargeStruct, 8>, LargeStruct>)->Name("Copy/Large_LargeStruct/SmallVector");
BENCHMARK(BM_Copy_Large<boost::container::static_vector<LargeStruct, 1024>, LargeStruct>)->Name("Copy/Large_LargeStruct/StaticVector");
BENCHMARK(BM_Copy_Large<absl::InlinedVector<LargeStruct, 8>, LargeStruct>)->Name("Copy/Large_LargeStruct/InlinedVector");

BENCHMARK_MAIN();
