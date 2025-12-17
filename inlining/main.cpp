#include <iostream>
#include <vector>
#include <string>
#include <benchmark/benchmark.h>

// #if defined(__GNUC__) || defined(__clang__)
//     #define FORCE_INLINE __attribute__((always_inline)) inline
// #endif

void some_func() {
    std::cout << "some_func" << std::endl;
}

// Benchmark for some_func()
static void BM_some_func(benchmark::State& state) {
    // Redirect cout to suppress output during benchmarking
    std::cout.setstate(std::ios_base::failbit);
    
    for (auto _ : state) {
        some_func();
    }
    
    // Re-enable cout
    std::cout.clear();
}

// Register the benchmark
BENCHMARK(BM_some_func);

// Main function to run benchmarks
BENCHMARK_MAIN();
