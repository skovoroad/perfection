#include <random>
#include <benchmark/benchmark.h>

// 16MB array (4 million integers) - larger than typical L3 cache
static constexpr size_t ARRAY_SIZE = 4 * 1024 * 1024;
static int data[ARRAY_SIZE];

void initialize_data() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        data[i] = dis(gen);
    }
}

// Sequential access - good cache locality
// Accesses memory in order, benefits from:
// - Spatial locality (nearby elements in same cache line)
// - Hardware prefetching (CPU can predict pattern)
// - Full cache line utilization
long long prfct_sum_sequential() {
    long long sum = 0;
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        sum += data[i];
    }
    return sum;
}

// Strided access - poor cache locality
// Jumps across memory with large stride (1024 elements = 4KB)
// This defeats:
// - Hardware prefetching (unpredictable pattern)
// - Cache efficiency (loads cache lines but uses small portion)
// - Memory bandwidth (more main memory accesses)
long long prfct_sum_strided() {
    constexpr size_t STRIDE = 1024; // 4KB jumps (typical page size)
    long long sum = 0;
    
    // Process all elements by cycling through offsets
    for (size_t offset = 0; offset < STRIDE; ++offset) {
        for (size_t i = offset; i < ARRAY_SIZE; i += STRIDE) {
            sum += data[i];
        }
    }
    
    return sum;
}

static void BM_sequential(benchmark::State& state) {
    initialize_data();
    
    for (auto _ : state) {
        long long result = prfct_sum_sequential();
        benchmark::DoNotOptimize(result);
    }
}

static void BM_strided(benchmark::State& state) {
    initialize_data();
    
    for (auto _ : state) {
        long long result = prfct_sum_strided();
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_sequential);
BENCHMARK(BM_strided);

BENCHMARK_MAIN();
