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
// CLEAR and REFILL Benchmarks
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

// =============================================================================
// Small + SmallElement (int)
// =============================================================================

BENCHMARK(BM_ClearRefill_Small<std::vector<SmallElement>, SmallElement>)->Name("ClearRefill/Small_int/StdVector");
BENCHMARK(BM_ClearRefill_Small<boost::container::vector<SmallElement>, SmallElement>)->Name("ClearRefill/Small_int/BoostVector");
BENCHMARK(BM_ClearRefill_Small<boost::container::small_vector<SmallElement, 8>, SmallElement>)->Name("ClearRefill/Small_int/SmallVector");
BENCHMARK(BM_ClearRefill_Small<boost::container::static_vector<SmallElement, 8>, SmallElement>)->Name("ClearRefill/Small_int/StaticVector");
BENCHMARK(BM_ClearRefill_Small<absl::InlinedVector<SmallElement, 8>, SmallElement>)->Name("ClearRefill/Small_int/InlinedVector");

BENCHMARK_MAIN();
