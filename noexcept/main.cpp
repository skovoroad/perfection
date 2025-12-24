#include <random>
#include <benchmark/benchmark.h>

static char random_data[1024*1024];

void initialize_random_data() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    for (size_t i = 0; i < sizeof(random_data); ++i) {
        random_data[i] = static_cast<char>(dis(gen));
    }
}

// Function with noexcept specifier
void swap_chars_noexcept(char& a, char& b) noexcept {
    std::swap(a, b);
}

void process_data_noexcept() {
    size_t size = sizeof(random_data);
    for (size_t i = 0; i < size / 2; ++i) {
        swap_chars_noexcept(random_data[i], random_data[size - 1 - i]);
    }
}

// Function without noexcept specifier
void swap_chars_regular(char& a, char& b) {
    std::swap(a, b);
}

void process_data_regular() {
    size_t size = sizeof(random_data);
    for (size_t i = 0; i < size / 2; ++i) {
        swap_chars_regular(random_data[i], random_data[size - 1 - i]);
    }
}

static void BM_noexcept(benchmark::State& state) {
    initialize_random_data();
    
    for (auto _ : state) {
        process_data_noexcept();
        benchmark::DoNotOptimize(random_data);
    }
}

static void BM_regular(benchmark::State& state) {
    initialize_random_data();
    
    for (auto _ : state) {
        process_data_regular();
        benchmark::DoNotOptimize(random_data);
    }
}

BENCHMARK(BM_noexcept);
BENCHMARK(BM_regular);

BENCHMARK_MAIN();
