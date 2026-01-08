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

BENCHMARK_MAIN();
