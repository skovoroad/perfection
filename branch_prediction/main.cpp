#include <random>
#include <benchmark/benchmark.h>

// Array of random numbers from 1 to 100
static constexpr size_t ARRAY_SIZE = 1024 * 1024; // 1M elements
static int data[ARRAY_SIZE];

void initialize_data() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        data[i] = dis(gen);
    }
}

// Predictable branch pattern:
// Pass 1: threshold 195 (almost always true: sum in [2, 200])
// Pass 2: threshold 195 (almost always false)
// Pattern: T T T T T ... F F F → highly predictable for branch predictor
void prfct_swap_predictable() {
    // Pass 1: sum < 195 (~97.5% true)
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] < 195) {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
    
    // Pass 2: sum > 195 (~2.5% true)
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] > 195) {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
}

// Unpredictable branch pattern:
// Pass 1: threshold 100 (~50% true)
// Pass 2: threshold 100 (~50% true)
// Pattern: T F T T F T F T ... → unpredictable, ~50% branch misses
void prfct_swap_unpredictable() {
    // Pass 1: sum < 100 (~49% true)
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] < 100) {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
    
    // Pass 2: sum >= 100 (~51% true)
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] >= 100) {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
}

static void BM_predictable(benchmark::State& state) {
    initialize_data();
    
    for (auto _ : state) {
        prfct_swap_predictable();
        benchmark::DoNotOptimize(data);
    }
}

static void BM_unpredictable(benchmark::State& state) {
    initialize_data();
    
    for (auto _ : state) {
        prfct_swap_unpredictable();
        benchmark::DoNotOptimize(data);
    }
}

BENCHMARK(BM_predictable);
BENCHMARK(BM_unpredictable);

BENCHMARK_MAIN();
