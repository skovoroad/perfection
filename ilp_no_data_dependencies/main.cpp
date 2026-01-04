#include <random>
#include <benchmark/benchmark.h>

// Array of random numbers
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

// Sequential: one swap per iteration (baseline)
void prfct_swap_sequential() {
    for (size_t i = 0; i < ARRAY_SIZE / 2; i++) {
        std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
    }
}

// Template declaration - only explicit specializations below should be used
template<size_t UnrollFactor>
void prfct_swap_unrolled() {
    static_assert(UnrollFactor == 0, "Only explicit specializations (4, 8, 16, 32) are implemented");
}

// Explicit specializations for better code generation
template<>
void prfct_swap_unrolled<4>() {
    for (size_t i = 0; i < ARRAY_SIZE / 2; i += 4) {
        std::swap(data[i + 0], data[ARRAY_SIZE - 1 - (i + 0)]);
        std::swap(data[i + 1], data[ARRAY_SIZE - 1 - (i + 1)]);
        std::swap(data[i + 2], data[ARRAY_SIZE - 1 - (i + 2)]);
        std::swap(data[i + 3], data[ARRAY_SIZE - 1 - (i + 3)]);
    }
}

template<>
void prfct_swap_unrolled<8>() {
    for (size_t i = 0; i < ARRAY_SIZE / 2; i += 8) {
        std::swap(data[i + 0], data[ARRAY_SIZE - 1 - (i + 0)]);
        std::swap(data[i + 1], data[ARRAY_SIZE - 1 - (i + 1)]);
        std::swap(data[i + 2], data[ARRAY_SIZE - 1 - (i + 2)]);
        std::swap(data[i + 3], data[ARRAY_SIZE - 1 - (i + 3)]);
        std::swap(data[i + 4], data[ARRAY_SIZE - 1 - (i + 4)]);
        std::swap(data[i + 5], data[ARRAY_SIZE - 1 - (i + 5)]);
        std::swap(data[i + 6], data[ARRAY_SIZE - 1 - (i + 6)]);
        std::swap(data[i + 7], data[ARRAY_SIZE - 1 - (i + 7)]);
    }
}

template<>
void prfct_swap_unrolled<16>() {
    for (size_t i = 0; i < ARRAY_SIZE / 2; i += 16) {
        std::swap(data[i + 0], data[ARRAY_SIZE - 1 - (i + 0)]);
        std::swap(data[i + 1], data[ARRAY_SIZE - 1 - (i + 1)]);
        std::swap(data[i + 2], data[ARRAY_SIZE - 1 - (i + 2)]);
        std::swap(data[i + 3], data[ARRAY_SIZE - 1 - (i + 3)]);
        std::swap(data[i + 4], data[ARRAY_SIZE - 1 - (i + 4)]);
        std::swap(data[i + 5], data[ARRAY_SIZE - 1 - (i + 5)]);
        std::swap(data[i + 6], data[ARRAY_SIZE - 1 - (i + 6)]);
        std::swap(data[i + 7], data[ARRAY_SIZE - 1 - (i + 7)]);
        std::swap(data[i + 8], data[ARRAY_SIZE - 1 - (i + 8)]);
        std::swap(data[i + 9], data[ARRAY_SIZE - 1 - (i + 9)]);
        std::swap(data[i + 10], data[ARRAY_SIZE - 1 - (i + 10)]);
        std::swap(data[i + 11], data[ARRAY_SIZE - 1 - (i + 11)]);
        std::swap(data[i + 12], data[ARRAY_SIZE - 1 - (i + 12)]);
        std::swap(data[i + 13], data[ARRAY_SIZE - 1 - (i + 13)]);
        std::swap(data[i + 14], data[ARRAY_SIZE - 1 - (i + 14)]);
        std::swap(data[i + 15], data[ARRAY_SIZE - 1 - (i + 15)]);
    }
}

template<>
void prfct_swap_unrolled<32>() {
    for (size_t i = 0; i < ARRAY_SIZE / 2; i += 32) {
        std::swap(data[i + 0], data[ARRAY_SIZE - 1 - (i + 0)]);
        std::swap(data[i + 1], data[ARRAY_SIZE - 1 - (i + 1)]);
        std::swap(data[i + 2], data[ARRAY_SIZE - 1 - (i + 2)]);
        std::swap(data[i + 3], data[ARRAY_SIZE - 1 - (i + 3)]);
        std::swap(data[i + 4], data[ARRAY_SIZE - 1 - (i + 4)]);
        std::swap(data[i + 5], data[ARRAY_SIZE - 1 - (i + 5)]);
        std::swap(data[i + 6], data[ARRAY_SIZE - 1 - (i + 6)]);
        std::swap(data[i + 7], data[ARRAY_SIZE - 1 - (i + 7)]);
        std::swap(data[i + 8], data[ARRAY_SIZE - 1 - (i + 8)]);
        std::swap(data[i + 9], data[ARRAY_SIZE - 1 - (i + 9)]);
        std::swap(data[i + 10], data[ARRAY_SIZE - 1 - (i + 10)]);
        std::swap(data[i + 11], data[ARRAY_SIZE - 1 - (i + 11)]);
        std::swap(data[i + 12], data[ARRAY_SIZE - 1 - (i + 12)]);
        std::swap(data[i + 13], data[ARRAY_SIZE - 1 - (i + 13)]);
        std::swap(data[i + 14], data[ARRAY_SIZE - 1 - (i + 14)]);
        std::swap(data[i + 15], data[ARRAY_SIZE - 1 - (i + 15)]);
        std::swap(data[i + 16], data[ARRAY_SIZE - 1 - (i + 16)]);
        std::swap(data[i + 17], data[ARRAY_SIZE - 1 - (i + 17)]);
        std::swap(data[i + 18], data[ARRAY_SIZE - 1 - (i + 18)]);
        std::swap(data[i + 19], data[ARRAY_SIZE - 1 - (i + 19)]);
        std::swap(data[i + 20], data[ARRAY_SIZE - 1 - (i + 20)]);
        std::swap(data[i + 21], data[ARRAY_SIZE - 1 - (i + 21)]);
        std::swap(data[i + 22], data[ARRAY_SIZE - 1 - (i + 22)]);
        std::swap(data[i + 23], data[ARRAY_SIZE - 1 - (i + 23)]);
        std::swap(data[i + 24], data[ARRAY_SIZE - 1 - (i + 24)]);
        std::swap(data[i + 25], data[ARRAY_SIZE - 1 - (i + 25)]);
        std::swap(data[i + 26], data[ARRAY_SIZE - 1 - (i + 26)]);
        std::swap(data[i + 27], data[ARRAY_SIZE - 1 - (i + 27)]);
        std::swap(data[i + 28], data[ARRAY_SIZE - 1 - (i + 28)]);
        std::swap(data[i + 29], data[ARRAY_SIZE - 1 - (i + 29)]);
        std::swap(data[i + 30], data[ARRAY_SIZE - 1 - (i + 30)]);
        std::swap(data[i + 31], data[ARRAY_SIZE - 1 - (i + 31)]);
    }
}

static void BM_sequential(benchmark::State& state) {
    initialize_data();
    
    for (auto _ : state) {
        prfct_swap_sequential();
        benchmark::DoNotOptimize(data);
    }
}

// Parameterized benchmark for different unroll factors
static void BM_unrolled(benchmark::State& state) {
    initialize_data();
    int unroll_factor = state.range(0);
    
    for (auto _ : state) {
        switch(unroll_factor) {
            case 4:  prfct_swap_unrolled<4>(); break;
            case 8:  prfct_swap_unrolled<8>(); break;
            case 16: prfct_swap_unrolled<16>(); break;
            case 32: prfct_swap_unrolled<32>(); break;
        }
        benchmark::DoNotOptimize(data);
    }
    state.SetLabel("unroll_" + std::to_string(unroll_factor));
}

BENCHMARK(BM_sequential);
BENCHMARK(BM_unrolled)->Arg(4)->Arg(8)->Arg(16)->Arg(32);

BENCHMARK_MAIN();
