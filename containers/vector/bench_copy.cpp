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

BENCHMARK_MAIN();
