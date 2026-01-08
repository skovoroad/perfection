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
static void BM_Insert_SmallVector(benchmark::State& state) {
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
static void BM_Insert_MediumVector(benchmark::State& state) {
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
static void BM_Insert_LargeVector(benchmark::State& state) {
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
// INSERT Benchmarks with reserve()
// =============================================================================

template<typename Container, typename Element>
static void BM_Insert_SmallVector_Reserved(benchmark::State& state) {
    for (auto _ : state) {
        Container vec;
        vec.reserve(8);
        for (int i = 0; i < 8; ++i) {
            vec.push_back(Element(i));
        }
        benchmark::DoNotOptimize(vec.data());
        benchmark::ClobberMemory();
    }
}

template<typename Container, typename Element>
static void BM_Insert_MediumVector_Reserved(benchmark::State& state) {
    for (auto _ : state) {
        Container vec;
        vec.reserve(64);
        for (int i = 0; i < 64; ++i) {
            vec.push_back(Element(i));
        }
        benchmark::DoNotOptimize(vec.data());
        benchmark::ClobberMemory();
    }
}

template<typename Container, typename Element>
static void BM_Insert_LargeVector_Reserved(benchmark::State& state) {
    for (auto _ : state) {
        Container vec;
        vec.reserve(1024);
        for (int i = 0; i < 1024; ++i) {
            vec.push_back(Element(i));
        }
        benchmark::DoNotOptimize(vec.data());
        benchmark::ClobberMemory();
    }
}

// =============================================================================
// SmallVector + SmallItem
// =============================================================================

BENCHMARK(BM_Insert_SmallVector<std::vector<SmallItem>, SmallItem>)->Name("Insert/SmallVector_SmallItem/StdVector");
BENCHMARK(BM_Insert_SmallVector<boost::container::vector<SmallItem>, SmallItem>)->Name("Insert/SmallVector_SmallItem/BoostVector");
BENCHMARK(BM_Insert_SmallVector<boost::container::small_vector<SmallItem, 8>, SmallItem>)->Name("Insert/SmallVector_SmallItem/SmallVector");
BENCHMARK(BM_Insert_SmallVector<boost::container::static_vector<SmallItem, 8>, SmallItem>)->Name("Insert/SmallVector_SmallItem/StaticVector");
BENCHMARK(BM_Insert_SmallVector<absl::InlinedVector<SmallItem, 8>, SmallItem>)->Name("Insert/SmallVector_SmallItem/InlinedVector");

BENCHMARK(BM_Insert_SmallVector_Reserved<std::vector<SmallItem>, SmallItem>)->Name("Insert/SmallVector_SmallItem/StdVector_Reserved");
BENCHMARK(BM_Insert_SmallVector_Reserved<boost::container::vector<SmallItem>, SmallItem>)->Name("Insert/SmallVector_SmallItem/BoostVector_Reserved");
BENCHMARK(BM_Insert_SmallVector_Reserved<boost::container::small_vector<SmallItem, 8>, SmallItem>)->Name("Insert/SmallVector_SmallItem/SmallVector_Reserved");
BENCHMARK(BM_Insert_SmallVector_Reserved<absl::InlinedVector<SmallItem, 8>, SmallItem>)->Name("Insert/SmallVector_SmallItem/InlinedVector_Reserved");

// =============================================================================
// SmallVector + MediumItem
// =============================================================================

BENCHMARK(BM_Insert_SmallVector<std::vector<MediumItem>, MediumItem>)->Name("Insert/SmallVector_MediumItem/StdVector");
BENCHMARK(BM_Insert_SmallVector<boost::container::vector<MediumItem>, MediumItem>)->Name("Insert/SmallVector_MediumItem/BoostVector");
BENCHMARK(BM_Insert_SmallVector<boost::container::small_vector<MediumItem, 8>, MediumItem>)->Name("Insert/SmallVector_MediumItem/SmallVector");
BENCHMARK(BM_Insert_SmallVector<boost::container::static_vector<MediumItem, 8>, MediumItem>)->Name("Insert/SmallVector_MediumItem/StaticVector");
BENCHMARK(BM_Insert_SmallVector<absl::InlinedVector<MediumItem, 8>, MediumItem>)->Name("Insert/SmallVector_MediumItem/InlinedVector");

BENCHMARK(BM_Insert_SmallVector_Reserved<std::vector<MediumItem>, MediumItem>)->Name("Insert/SmallVector_MediumItem/StdVector_Reserved");
BENCHMARK(BM_Insert_SmallVector_Reserved<boost::container::vector<MediumItem>, MediumItem>)->Name("Insert/SmallVector_MediumItem/BoostVector_Reserved");
BENCHMARK(BM_Insert_SmallVector_Reserved<boost::container::small_vector<MediumItem, 8>, MediumItem>)->Name("Insert/SmallVector_MediumItem/SmallVector_Reserved");
BENCHMARK(BM_Insert_SmallVector_Reserved<absl::InlinedVector<MediumItem, 8>, MediumItem>)->Name("Insert/SmallVector_MediumItem/InlinedVector_Reserved");

// =============================================================================
// SmallVector + LargeItem
// =============================================================================

BENCHMARK(BM_Insert_SmallVector<std::vector<LargeItem>, LargeItem>)->Name("Insert/SmallVector_LargeItem/StdVector");
BENCHMARK(BM_Insert_SmallVector<boost::container::vector<LargeItem>, LargeItem>)->Name("Insert/SmallVector_LargeItem/BoostVector");
BENCHMARK(BM_Insert_SmallVector<boost::container::small_vector<LargeItem, 8>, LargeItem>)->Name("Insert/SmallVector_LargeItem/SmallVector");
BENCHMARK(BM_Insert_SmallVector<boost::container::static_vector<LargeItem, 8>, LargeItem>)->Name("Insert/SmallVector_LargeItem/StaticVector");
BENCHMARK(BM_Insert_SmallVector<absl::InlinedVector<LargeItem, 8>, LargeItem>)->Name("Insert/SmallVector_LargeItem/InlinedVector");

BENCHMARK(BM_Insert_SmallVector_Reserved<std::vector<LargeItem>, LargeItem>)->Name("Insert/SmallVector_LargeItem/StdVector_Reserved");
BENCHMARK(BM_Insert_SmallVector_Reserved<boost::container::vector<LargeItem>, LargeItem>)->Name("Insert/SmallVector_LargeItem/BoostVector_Reserved");
BENCHMARK(BM_Insert_SmallVector_Reserved<boost::container::small_vector<LargeItem, 8>, LargeItem>)->Name("Insert/SmallVector_LargeItem/SmallVector_Reserved");
BENCHMARK(BM_Insert_SmallVector_Reserved<absl::InlinedVector<LargeItem, 8>, LargeItem>)->Name("Insert/SmallVector_LargeItem/InlinedVector_Reserved");

// =============================================================================
// MediumVector + SmallItem
// =============================================================================

BENCHMARK(BM_Insert_MediumVector<std::vector<SmallItem>, SmallItem>)->Name("Insert/MediumVector_SmallItem/StdVector");
BENCHMARK(BM_Insert_MediumVector<boost::container::vector<SmallItem>, SmallItem>)->Name("Insert/MediumVector_SmallItem/BoostVector");
BENCHMARK(BM_Insert_MediumVector<boost::container::small_vector<SmallItem, 8>, SmallItem>)->Name("Insert/MediumVector_SmallItem/SmallVector");
BENCHMARK(BM_Insert_MediumVector<boost::container::static_vector<SmallItem, 64>, SmallItem>)->Name("Insert/MediumVector_SmallItem/StaticVector");
BENCHMARK(BM_Insert_MediumVector<absl::InlinedVector<SmallItem, 8>, SmallItem>)->Name("Insert/MediumVector_SmallItem/InlinedVector");

BENCHMARK(BM_Insert_MediumVector_Reserved<std::vector<SmallItem>, SmallItem>)->Name("Insert/MediumVector_SmallItem/StdVector_Reserved");
BENCHMARK(BM_Insert_MediumVector_Reserved<boost::container::vector<SmallItem>, SmallItem>)->Name("Insert/MediumVector_SmallItem/BoostVector_Reserved");
BENCHMARK(BM_Insert_MediumVector_Reserved<boost::container::small_vector<SmallItem, 8>, SmallItem>)->Name("Insert/MediumVector_SmallItem/SmallVector_Reserved");
BENCHMARK(BM_Insert_MediumVector_Reserved<absl::InlinedVector<SmallItem, 8>, SmallItem>)->Name("Insert/MediumVector_SmallItem/InlinedVector_Reserved");

// =============================================================================
// MediumVector + MediumItem
// =============================================================================

BENCHMARK(BM_Insert_MediumVector<std::vector<MediumItem>, MediumItem>)->Name("Insert/MediumVector_MediumItem/StdVector");
BENCHMARK(BM_Insert_MediumVector<boost::container::vector<MediumItem>, MediumItem>)->Name("Insert/MediumVector_MediumItem/BoostVector");
BENCHMARK(BM_Insert_MediumVector<boost::container::small_vector<MediumItem, 8>, MediumItem>)->Name("Insert/MediumVector_MediumItem/SmallVector");
BENCHMARK(BM_Insert_MediumVector<boost::container::static_vector<MediumItem, 64>, MediumItem>)->Name("Insert/MediumVector_MediumItem/StaticVector");
BENCHMARK(BM_Insert_MediumVector<absl::InlinedVector<MediumItem, 8>, MediumItem>)->Name("Insert/MediumVector_MediumItem/InlinedVector");

BENCHMARK(BM_Insert_MediumVector_Reserved<std::vector<MediumItem>, MediumItem>)->Name("Insert/MediumVector_MediumItem/StdVector_Reserved");
BENCHMARK(BM_Insert_MediumVector_Reserved<boost::container::vector<MediumItem>, MediumItem>)->Name("Insert/MediumVector_MediumItem/BoostVector_Reserved");
BENCHMARK(BM_Insert_MediumVector_Reserved<boost::container::small_vector<MediumItem, 8>, MediumItem>)->Name("Insert/MediumVector_MediumItem/SmallVector_Reserved");
BENCHMARK(BM_Insert_MediumVector_Reserved<absl::InlinedVector<MediumItem, 8>, MediumItem>)->Name("Insert/MediumVector_MediumItem/InlinedVector_Reserved");

// =============================================================================
// MediumVector + LargeItem
// =============================================================================

BENCHMARK(BM_Insert_MediumVector<std::vector<LargeItem>, LargeItem>)->Name("Insert/MediumVector_LargeItem/StdVector");
BENCHMARK(BM_Insert_MediumVector<boost::container::vector<LargeItem>, LargeItem>)->Name("Insert/MediumVector_LargeItem/BoostVector");
BENCHMARK(BM_Insert_MediumVector<boost::container::small_vector<LargeItem, 8>, LargeItem>)->Name("Insert/MediumVector_LargeItem/SmallVector");
BENCHMARK(BM_Insert_MediumVector<boost::container::static_vector<LargeItem, 64>, LargeItem>)->Name("Insert/MediumVector_LargeItem/StaticVector");
BENCHMARK(BM_Insert_MediumVector<absl::InlinedVector<LargeItem, 8>, LargeItem>)->Name("Insert/MediumVector_LargeItem/InlinedVector");

BENCHMARK(BM_Insert_MediumVector_Reserved<std::vector<LargeItem>, LargeItem>)->Name("Insert/MediumVector_LargeItem/StdVector_Reserved");
BENCHMARK(BM_Insert_MediumVector_Reserved<boost::container::vector<LargeItem>, LargeItem>)->Name("Insert/MediumVector_LargeItem/BoostVector_Reserved");
BENCHMARK(BM_Insert_MediumVector_Reserved<boost::container::small_vector<LargeItem, 8>, LargeItem>)->Name("Insert/MediumVector_LargeItem/SmallVector_Reserved");
BENCHMARK(BM_Insert_MediumVector_Reserved<absl::InlinedVector<LargeItem, 8>, LargeItem>)->Name("Insert/MediumVector_LargeItem/InlinedVector_Reserved");

// =============================================================================
// LargeVector + SmallItem
// =============================================================================

BENCHMARK(BM_Insert_LargeVector<std::vector<SmallItem>, SmallItem>)->Name("Insert/LargeVector_SmallItem/StdVector");
BENCHMARK(BM_Insert_LargeVector<boost::container::vector<SmallItem>, SmallItem>)->Name("Insert/LargeVector_SmallItem/BoostVector");
BENCHMARK(BM_Insert_LargeVector<boost::container::small_vector<SmallItem, 8>, SmallItem>)->Name("Insert/LargeVector_SmallItem/SmallVector");
BENCHMARK(BM_Insert_LargeVector<boost::container::static_vector<SmallItem, 1024>, SmallItem>)->Name("Insert/LargeVector_SmallItem/StaticVector");
BENCHMARK(BM_Insert_LargeVector<absl::InlinedVector<SmallItem, 8>, SmallItem>)->Name("Insert/LargeVector_SmallItem/InlinedVector");

BENCHMARK(BM_Insert_LargeVector_Reserved<std::vector<SmallItem>, SmallItem>)->Name("Insert/LargeVector_SmallItem/StdVector_Reserved");
BENCHMARK(BM_Insert_LargeVector_Reserved<boost::container::vector<SmallItem>, SmallItem>)->Name("Insert/LargeVector_SmallItem/BoostVector_Reserved");
BENCHMARK(BM_Insert_LargeVector_Reserved<boost::container::small_vector<SmallItem, 8>, SmallItem>)->Name("Insert/LargeVector_SmallItem/SmallVector_Reserved");
BENCHMARK(BM_Insert_LargeVector_Reserved<absl::InlinedVector<SmallItem, 8>, SmallItem>)->Name("Insert/LargeVector_SmallItem/InlinedVector_Reserved");

// =============================================================================
// LargeVector + MediumItem
// =============================================================================

BENCHMARK(BM_Insert_LargeVector<std::vector<MediumItem>, MediumItem>)->Name("Insert/LargeVector_MediumItem/StdVector");
BENCHMARK(BM_Insert_LargeVector<boost::container::vector<MediumItem>, MediumItem>)->Name("Insert/LargeVector_MediumItem/BoostVector");
BENCHMARK(BM_Insert_LargeVector<boost::container::small_vector<MediumItem, 8>, MediumItem>)->Name("Insert/LargeVector_MediumItem/SmallVector");
BENCHMARK(BM_Insert_LargeVector<boost::container::static_vector<MediumItem, 1024>, MediumItem>)->Name("Insert/LargeVector_MediumItem/StaticVector");
BENCHMARK(BM_Insert_LargeVector<absl::InlinedVector<MediumItem, 8>, MediumItem>)->Name("Insert/LargeVector_MediumItem/InlinedVector");

BENCHMARK(BM_Insert_LargeVector_Reserved<std::vector<MediumItem>, MediumItem>)->Name("Insert/LargeVector_MediumItem/StdVector_Reserved");
BENCHMARK(BM_Insert_LargeVector_Reserved<boost::container::vector<MediumItem>, MediumItem>)->Name("Insert/LargeVector_MediumItem/BoostVector_Reserved");
BENCHMARK(BM_Insert_LargeVector_Reserved<boost::container::small_vector<MediumItem, 8>, MediumItem>)->Name("Insert/LargeVector_MediumItem/SmallVector_Reserved");
BENCHMARK(BM_Insert_LargeVector_Reserved<absl::InlinedVector<MediumItem, 8>, MediumItem>)->Name("Insert/LargeVector_MediumItem/InlinedVector_Reserved");

// =============================================================================
// LargeVector + LargeItem
// =============================================================================

BENCHMARK(BM_Insert_LargeVector<std::vector<LargeItem>, LargeItem>)->Name("Insert/LargeVector_LargeItem/StdVector");
BENCHMARK(BM_Insert_LargeVector<boost::container::vector<LargeItem>, LargeItem>)->Name("Insert/LargeVector_LargeItem/BoostVector");
BENCHMARK(BM_Insert_LargeVector<boost::container::small_vector<LargeItem, 8>, LargeItem>)->Name("Insert/LargeVector_LargeItem/SmallVector");
BENCHMARK(BM_Insert_LargeVector<boost::container::static_vector<LargeItem, 1024>, LargeItem>)->Name("Insert/LargeVector_LargeItem/StaticVector");
BENCHMARK(BM_Insert_LargeVector<absl::InlinedVector<LargeItem, 8>, LargeItem>)->Name("Insert/LargeVector_LargeItem/InlinedVector");

BENCHMARK(BM_Insert_LargeVector_Reserved<std::vector<LargeItem>, LargeItem>)->Name("Insert/LargeVector_LargeItem/StdVector_Reserved");
BENCHMARK(BM_Insert_LargeVector_Reserved<boost::container::vector<LargeItem>, LargeItem>)->Name("Insert/LargeVector_LargeItem/BoostVector_Reserved");
BENCHMARK(BM_Insert_LargeVector_Reserved<boost::container::small_vector<LargeItem, 8>, LargeItem>)->Name("Insert/LargeVector_LargeItem/SmallVector_Reserved");
BENCHMARK(BM_Insert_LargeVector_Reserved<absl::InlinedVector<LargeItem, 8>, LargeItem>)->Name("Insert/LargeVector_LargeItem/InlinedVector_Reserved");

BENCHMARK_MAIN();
