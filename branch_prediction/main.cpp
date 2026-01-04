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

// ========== BASELINE: No attributes ==========

// Predictable: ~97% true, highly predictable for branch predictor
void prfct_swap_predictable() {
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] < 195) {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
    
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] > 195) {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
}

// Unpredictable: ~50% true, causes branch mispredictions
void prfct_swap_unpredictable() {
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] < 100) {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
    
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] >= 100) {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
}

// ========== PREDICTABLE + [[likely]]: Correct hint ==========
// Hint matches reality: branch is ~97% taken
// Expected: Helps with code layout, slight improvement
void prfct_swap_predictable_likely() {
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] < 195) [[likely]] {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
    
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] > 195) [[unlikely]] {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
}

// ========== PREDICTABLE + [[unlikely]]: WRONG hint ==========
// Hint contradicts reality: branch is ~97% taken but marked unlikely
// Expected: Hurts code layout, performance degradation
void prfct_swap_predictable_unlikely() {
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] < 195) [[unlikely]] {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
    
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] > 195) [[likely]] {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
}

// ========== UNPREDICTABLE + [[likely]]: WRONG hint ==========
// Hint contradicts reality: branch is ~50% but marked likely
// Expected: Hurts code layout, performance degradation
void prfct_swap_unpredictable_likely() {
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] < 100) [[likely]] {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
    
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] >= 100) [[likely]] {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
}

// ========== UNPREDICTABLE + [[unlikely]]: WRONG hint ==========
// Hint contradicts reality: branch is ~50% but marked unlikely
// Expected: Hurts code layout, performance degradation
void prfct_swap_unpredictable_unlikely() {
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] < 100) [[unlikely]] {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
    
    for (size_t i = 0; i < ARRAY_SIZE / 2; ++i) {
        if (data[i] + data[ARRAY_SIZE - 1 - i] >= 100) [[unlikely]] {
            std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        }
    }
}

// ========== BENCHMARKS ==========

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

static void BM_predictable_likely(benchmark::State& state) {
    initialize_data();
    for (auto _ : state) {
        prfct_swap_predictable_likely();
        benchmark::DoNotOptimize(data);
    }
}

static void BM_predictable_unlikely(benchmark::State& state) {
    initialize_data();
    for (auto _ : state) {
        prfct_swap_predictable_unlikely();
        benchmark::DoNotOptimize(data);
    }
}

static void BM_unpredictable_likely(benchmark::State& state) {
    initialize_data();
    for (auto _ : state) {
        prfct_swap_unpredictable_likely();
        benchmark::DoNotOptimize(data);
    }
}

static void BM_unpredictable_unlikely(benchmark::State& state) {
    initialize_data();
    for (auto _ : state) {
        prfct_swap_unpredictable_unlikely();
        benchmark::DoNotOptimize(data);
    }
}

// Baseline
BENCHMARK(BM_predictable);
BENCHMARK(BM_unpredictable);

// Correct hints
BENCHMARK(BM_predictable_likely);

// Wrong hints
BENCHMARK(BM_predictable_unlikely);
BENCHMARK(BM_unpredictable_likely);
BENCHMARK(BM_unpredictable_unlikely);

BENCHMARK_MAIN();
