#include <random>
#include <benchmark/benchmark.h>

#if defined(__GNUC__) || defined(__clang__)
    #define FORCE_INLINE __attribute__((always_inline)) inline
    #define NOINLINE __attribute__((noinline))
#endif

static char random_data[1024*1024];

void initialize_random_data() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    for (size_t i = 0; i < sizeof(random_data); ++i) {
        random_data[i] = static_cast<char>(dis(gen));
    }
}

FORCE_INLINE void swap_chars_inlined(char& a, char& b) {
    std::swap(a, b);
}

void process_random_data_inlined() {
    size_t size = sizeof(random_data);
    for (size_t i = 0; i < size / 2; ++i) {
        swap_chars_inlined(random_data[i], random_data[size - 1 - i]);
    }
}

NOINLINE void swap_chars_noinline(char& a, char& b) {
    std::swap(a, b);
}

void process_random_data_noinline() {
    size_t size = sizeof(random_data);
    for (size_t i = 0; i < size / 2; ++i) {
        swap_chars_noinline(random_data[i], random_data[size - 1 - i]);
    }
}

static void BM_process_inlined(benchmark::State& state) {
    initialize_random_data();
    
    for (auto _ : state) {
        process_random_data_inlined();
        benchmark::DoNotOptimize(random_data);
    }
}

static void BM_process_noinline(benchmark::State& state) {
    initialize_random_data();
    
    for (auto _ : state) {
        process_random_data_noinline();
        benchmark::DoNotOptimize(random_data);
    }
}

BENCHMARK(BM_process_inlined);
BENCHMARK(BM_process_noinline);

BENCHMARK_MAIN();
