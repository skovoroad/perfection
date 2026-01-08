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
// INSERT Benchmarks
// =============================================================================

template<typename Container, typename Element>
static void BM_Insert_Small(benchmark::State& state) {
    for (auto _ : state) {
        Container vec;
        for (int i = 0; i < 8; ++i) {
            vec.push_back(Element(i));
        }
        benchmark::DoNotOptimize(vec.data());
        benchmark::ClobberMemory();
    }
}

template<typename Container, typename Element>
static void BM_Insert_Medium(benchmark::State& state) {
    for (auto _ : state) {
        Container vec;
        for (int i = 0; i < 64; ++i) {
            vec.push_back(Element(i));
        }
        benchmark::DoNotOptimize(vec.data());
        benchmark::ClobberMemory();
    }
}

template<typename Container, typename Element>
static void BM_Insert_Large(benchmark::State& state) {
    for (auto _ : state) {
        Container vec;
        for (int i = 0; i < 1024; ++i) {
            vec.push_back(Element(i));
        }
        benchmark::DoNotOptimize(vec.data());
        benchmark::ClobberMemory();
    }
}

// =============================================================================
// Small + SmallElement (int)
// =============================================================================

BENCHMARK(BM_Insert_Small<std::vector<SmallElement>, SmallElement>)->Name("Insert/Small_int/StdVector");
BENCHMARK(BM_Insert_Small<boost::container::vector<SmallElement>, SmallElement>)->Name("Insert/Small_int/BoostVector");
BENCHMARK(BM_Insert_Small<boost::container::small_vector<SmallElement, 8>, SmallElement>)->Name("Insert/Small_int/SmallVector");
BENCHMARK(BM_Insert_Small<boost::container::static_vector<SmallElement, 8>, SmallElement>)->Name("Insert/Small_int/StaticVector");
BENCHMARK(BM_Insert_Small<absl::InlinedVector<SmallElement, 8>, SmallElement>)->Name("Insert/Small_int/InlinedVector");

// =============================================================================
// Small + Point
// =============================================================================

BENCHMARK(BM_Insert_Small<std::vector<Point>, Point>)->Name("Insert/Small_Point/StdVector");
BENCHMARK(BM_Insert_Small<boost::container::vector<Point>, Point>)->Name("Insert/Small_Point/BoostVector");
BENCHMARK(BM_Insert_Small<boost::container::small_vector<Point, 8>, Point>)->Name("Insert/Small_Point/SmallVector");
BENCHMARK(BM_Insert_Small<boost::container::static_vector<Point, 8>, Point>)->Name("Insert/Small_Point/StaticVector");
BENCHMARK(BM_Insert_Small<absl::InlinedVector<Point, 8>, Point>)->Name("Insert/Small_Point/InlinedVector");

// =============================================================================
// Small + LargeStruct
// =============================================================================

BENCHMARK(BM_Insert_Small<std::vector<LargeStruct>, LargeStruct>)->Name("Insert/Small_LargeStruct/StdVector");
BENCHMARK(BM_Insert_Small<boost::container::vector<LargeStruct>, LargeStruct>)->Name("Insert/Small_LargeStruct/BoostVector");
BENCHMARK(BM_Insert_Small<boost::container::small_vector<LargeStruct, 8>, LargeStruct>)->Name("Insert/Small_LargeStruct/SmallVector");
BENCHMARK(BM_Insert_Small<boost::container::static_vector<LargeStruct, 8>, LargeStruct>)->Name("Insert/Small_LargeStruct/StaticVector");
BENCHMARK(BM_Insert_Small<absl::InlinedVector<LargeStruct, 8>, LargeStruct>)->Name("Insert/Small_LargeStruct/InlinedVector");

// =============================================================================
// Medium + SmallElement (int)
// =============================================================================

BENCHMARK(BM_Insert_Medium<std::vector<SmallElement>, SmallElement>)->Name("Insert/Medium_int/StdVector");
BENCHMARK(BM_Insert_Medium<boost::container::vector<SmallElement>, SmallElement>)->Name("Insert/Medium_int/BoostVector");
BENCHMARK(BM_Insert_Medium<boost::container::small_vector<SmallElement, 8>, SmallElement>)->Name("Insert/Medium_int/SmallVector");
BENCHMARK(BM_Insert_Medium<boost::container::static_vector<SmallElement, 64>, SmallElement>)->Name("Insert/Medium_int/StaticVector");
BENCHMARK(BM_Insert_Medium<absl::InlinedVector<SmallElement, 8>, SmallElement>)->Name("Insert/Medium_int/InlinedVector");

// =============================================================================
// Medium + Point
// =============================================================================

BENCHMARK(BM_Insert_Medium<std::vector<Point>, Point>)->Name("Insert/Medium_Point/StdVector");
BENCHMARK(BM_Insert_Medium<boost::container::vector<Point>, Point>)->Name("Insert/Medium_Point/BoostVector");
BENCHMARK(BM_Insert_Medium<boost::container::small_vector<Point, 8>, Point>)->Name("Insert/Medium_Point/SmallVector");
BENCHMARK(BM_Insert_Medium<boost::container::static_vector<Point, 64>, Point>)->Name("Insert/Medium_Point/StaticVector");
BENCHMARK(BM_Insert_Medium<absl::InlinedVector<Point, 8>, Point>)->Name("Insert/Medium_Point/InlinedVector");

// =============================================================================
// Medium + LargeStruct
// =============================================================================

BENCHMARK(BM_Insert_Medium<std::vector<LargeStruct>, LargeStruct>)->Name("Insert/Medium_LargeStruct/StdVector");
BENCHMARK(BM_Insert_Medium<boost::container::vector<LargeStruct>, LargeStruct>)->Name("Insert/Medium_LargeStruct/BoostVector");
BENCHMARK(BM_Insert_Medium<boost::container::small_vector<LargeStruct, 8>, LargeStruct>)->Name("Insert/Medium_LargeStruct/SmallVector");
BENCHMARK(BM_Insert_Medium<boost::container::static_vector<LargeStruct, 64>, LargeStruct>)->Name("Insert/Medium_LargeStruct/StaticVector");
BENCHMARK(BM_Insert_Medium<absl::InlinedVector<LargeStruct, 8>, LargeStruct>)->Name("Insert/Medium_LargeStruct/InlinedVector");

// =============================================================================
// Large + SmallElement (int)
// =============================================================================

BENCHMARK(BM_Insert_Large<std::vector<SmallElement>, SmallElement>)->Name("Insert/Large_int/StdVector");
BENCHMARK(BM_Insert_Large<boost::container::vector<SmallElement>, SmallElement>)->Name("Insert/Large_int/BoostVector");
BENCHMARK(BM_Insert_Large<boost::container::small_vector<SmallElement, 8>, SmallElement>)->Name("Insert/Large_int/SmallVector");
BENCHMARK(BM_Insert_Large<boost::container::static_vector<SmallElement, 1024>, SmallElement>)->Name("Insert/Large_int/StaticVector");
BENCHMARK(BM_Insert_Large<absl::InlinedVector<SmallElement, 8>, SmallElement>)->Name("Insert/Large_int/InlinedVector");

// =============================================================================
// Large + Point
// =============================================================================

BENCHMARK(BM_Insert_Large<std::vector<Point>, Point>)->Name("Insert/Large_Point/StdVector");
BENCHMARK(BM_Insert_Large<boost::container::vector<Point>, Point>)->Name("Insert/Large_Point/BoostVector");
BENCHMARK(BM_Insert_Large<boost::container::small_vector<Point, 8>, Point>)->Name("Insert/Large_Point/SmallVector");
BENCHMARK(BM_Insert_Large<boost::container::static_vector<Point, 1024>, Point>)->Name("Insert/Large_Point/StaticVector");
BENCHMARK(BM_Insert_Large<absl::InlinedVector<Point, 8>, Point>)->Name("Insert/Large_Point/InlinedVector");

// =============================================================================
// Large + LargeStruct
// =============================================================================

BENCHMARK(BM_Insert_Large<std::vector<LargeStruct>, LargeStruct>)->Name("Insert/Large_LargeStruct/StdVector");
BENCHMARK(BM_Insert_Large<boost::container::vector<LargeStruct>, LargeStruct>)->Name("Insert/Large_LargeStruct/BoostVector");
BENCHMARK(BM_Insert_Large<boost::container::small_vector<LargeStruct, 8>, LargeStruct>)->Name("Insert/Large_LargeStruct/SmallVector");
BENCHMARK(BM_Insert_Large<boost::container::static_vector<LargeStruct, 1024>, LargeStruct>)->Name("Insert/Large_LargeStruct/StaticVector");
BENCHMARK(BM_Insert_Large<absl::InlinedVector<LargeStruct, 8>, LargeStruct>)->Name("Insert/Large_LargeStruct/InlinedVector");

BENCHMARK_MAIN();
