#include <benchmark/benchmark.h>
#include <vector>
#include <array>

// Boost containers
#include <boost/container/vector.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/container/static_vector.hpp>

// Abseil container
#include <absl/container/inlined_vector.h>

// =============================================================================
// Test Data Structures
// =============================================================================

// Small element: 4 bytes
using SmallElement = int;

// Medium element: 24 bytes
struct Point {
    double x, y, z;
    Point() : x(0), y(0), z(0) {}
    explicit Point(int val) : x(val), y(val), z(val) {}
};

// Large element: 256 bytes
struct LargeStruct {
    std::array<int, 64> data;
    LargeStruct() { data.fill(0); }
    explicit LargeStruct(int val) { data.fill(val); }
};

// =============================================================================
// Benchmark: INSERT - Small containers (8 elements)
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

// Insert Small - SmallElement (all containers)
BENCHMARK(BM_Insert_Small<std::vector<SmallElement>, SmallElement>);
BENCHMARK(BM_Insert_Small<boost::container::vector<SmallElement>, SmallElement>);
BENCHMARK(BM_Insert_Small<boost::container::small_vector<SmallElement, 8>, SmallElement>);
BENCHMARK(BM_Insert_Small<boost::container::static_vector<SmallElement, 8>, SmallElement>);
BENCHMARK(BM_Insert_Small<absl::InlinedVector<SmallElement, 8>, SmallElement>);

// Insert Small - Point (all containers)
BENCHMARK(BM_Insert_Small<std::vector<Point>, Point>);
BENCHMARK(BM_Insert_Small<boost::container::vector<Point>, Point>);
BENCHMARK(BM_Insert_Small<boost::container::small_vector<Point, 8>, Point>);
BENCHMARK(BM_Insert_Small<boost::container::static_vector<Point, 8>, Point>);
BENCHMARK(BM_Insert_Small<absl::InlinedVector<Point, 8>, Point>);

// Insert Small - LargeStruct (all containers)
BENCHMARK(BM_Insert_Small<std::vector<LargeStruct>, LargeStruct>);
BENCHMARK(BM_Insert_Small<boost::container::vector<LargeStruct>, LargeStruct>);
BENCHMARK(BM_Insert_Small<boost::container::small_vector<LargeStruct, 8>, LargeStruct>);
BENCHMARK(BM_Insert_Small<boost::container::static_vector<LargeStruct, 8>, LargeStruct>);
BENCHMARK(BM_Insert_Small<absl::InlinedVector<LargeStruct, 8>, LargeStruct>);

// =============================================================================
// Benchmark: INSERT - Medium containers (64 elements)
// =============================================================================

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

// Insert Medium - SmallElement (all containers)
BENCHMARK(BM_Insert_Medium<std::vector<SmallElement>, SmallElement>);
BENCHMARK(BM_Insert_Medium<boost::container::vector<SmallElement>, SmallElement>);
BENCHMARK(BM_Insert_Medium<boost::container::small_vector<SmallElement, 8>, SmallElement>);
BENCHMARK(BM_Insert_Medium<absl::InlinedVector<SmallElement, 8>, SmallElement>);

// Insert Medium - Point (all containers)
BENCHMARK(BM_Insert_Medium<std::vector<Point>, Point>);
BENCHMARK(BM_Insert_Medium<boost::container::vector<Point>, Point>);
BENCHMARK(BM_Insert_Medium<boost::container::small_vector<Point, 8>, Point>);
BENCHMARK(BM_Insert_Medium<absl::InlinedVector<Point, 8>, Point>);

// =============================================================================
// Benchmark: INSERT - Large containers (1024 elements)
// =============================================================================

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

// Insert Large - SmallElement (all containers)
BENCHMARK(BM_Insert_Large<std::vector<SmallElement>, SmallElement>);
BENCHMARK(BM_Insert_Large<boost::container::vector<SmallElement>, SmallElement>);
BENCHMARK(BM_Insert_Large<boost::container::small_vector<SmallElement, 8>, SmallElement>);
BENCHMARK(BM_Insert_Large<absl::InlinedVector<SmallElement, 8>, SmallElement>);

// Insert Large - Point (all containers)
BENCHMARK(BM_Insert_Large<std::vector<Point>, Point>);
BENCHMARK(BM_Insert_Large<boost::container::vector<Point>, Point>);
BENCHMARK(BM_Insert_Large<boost::container::small_vector<Point, 8>, Point>);
BENCHMARK(BM_Insert_Large<absl::InlinedVector<Point, 8>, Point>);

// =============================================================================
// Benchmark: ITERATION - Sequential access
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

BENCHMARK(BM_Iterate_Small<std::vector<SmallElement>, SmallElement>);
BENCHMARK(BM_Iterate_Small<boost::container::vector<SmallElement>, SmallElement>);
BENCHMARK(BM_Iterate_Small<boost::container::small_vector<SmallElement, 8>, SmallElement>);
BENCHMARK(BM_Iterate_Small<boost::container::static_vector<SmallElement, 8>, SmallElement>);
BENCHMARK(BM_Iterate_Small<absl::InlinedVector<SmallElement, 8>, SmallElement>);

// =============================================================================
// Benchmark: COPY container
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

// Copy Small - SmallElement (all containers)
BENCHMARK(BM_Copy_Small<std::vector<SmallElement>, SmallElement>);
BENCHMARK(BM_Copy_Small<boost::container::vector<SmallElement>, SmallElement>);
BENCHMARK(BM_Copy_Small<boost::container::small_vector<SmallElement, 8>, SmallElement>);
BENCHMARK(BM_Copy_Small<boost::container::static_vector<SmallElement, 8>, SmallElement>);
BENCHMARK(BM_Copy_Small<absl::InlinedVector<SmallElement, 8>, SmallElement>);

// Copy Small - Point (all containers)
BENCHMARK(BM_Copy_Small<std::vector<Point>, Point>);
BENCHMARK(BM_Copy_Small<boost::container::small_vector<Point, 8>, Point>);
BENCHMARK(BM_Copy_Small<boost::container::static_vector<Point, 8>, Point>);
BENCHMARK(BM_Copy_Small<absl::InlinedVector<Point, 8>, Point>);

// =============================================================================
// Benchmark: CLEAR and REFILL
// =============================================================================

template<typename Container, typename Element>
static void BM_ClearRefill_Small(benchmark::State& state) {
    Container vec;
    
    for (auto _ : state) {
        vec.clear();
        for (int i = 0; i < 8; ++i) {
            vec.push_back(Element(i));
        }
        benchmark::DoNotOptimize(vec.data());
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_ClearRefill_Small<std::vector<SmallElement>, SmallElement>);
BENCHMARK(BM_ClearRefill_Small<boost::container::vector<SmallElement>, SmallElement>);
BENCHMARK(BM_ClearRefill_Small<boost::container::small_vector<SmallElement, 8>, SmallElement>);
BENCHMARK(BM_ClearRefill_Small<boost::container::static_vector<SmallElement, 8>, SmallElement>);
BENCHMARK(BM_ClearRefill_Small<absl::InlinedVector<SmallElement, 8>, SmallElement>);

// Main function
BENCHMARK_MAIN();
