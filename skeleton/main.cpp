#include <benchmark/benchmark.h>

void skeleton() {
    // TODO: Add your implementation here
}

void skeleton_optimized() {
    // TODO: Add your optimized implementation here
}

static void BM_skeleton(benchmark::State& state) {
    for (auto _ : state) {
        skeleton();
    }
}

static void BM_skeleton_optimized(benchmark::State& state) {
    for (auto _ : state) {
        skeleton_optimized();
    }
}

BENCHMARK(BM_skeleton);
BENCHMARK(BM_skeleton_optimized);

BENCHMARK_MAIN();
