#include <random>
#include <stdexcept>
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

// Version with return code
bool prfct_swap_chars_return_code(char& a, char& b) {
    // Only swap if both values are even or both are odd
    bool a_even = (static_cast<unsigned char>(a) % 2) == 0;
    bool b_even = (static_cast<unsigned char>(b) % 2) == 0;
    
    if (a_even == b_even) {
        std::swap(a, b);
        return true;
    }
    return false;
}

void prfct_process_random_data_return_code() {
    size_t size = sizeof(random_data);
    [[maybe_unused]] size_t error_count = 0;
    
    for (size_t i = 0; i < size / 2; ++i) {
        if (!prfct_swap_chars_return_code(random_data[i], random_data[size - 1 - i])) {
            error_count++;
        }
    }
}

// Version with exception
void prfct_swap_chars_exception(char& a, char& b) {
    // Only swap if both values are even or both are odd
    bool a_even = (static_cast<unsigned char>(a) % 2) == 0;
    bool b_even = (static_cast<unsigned char>(b) % 2) == 0;
    
    if (a_even == b_even) {
        std::swap(a, b);
    } else {
        throw std::runtime_error("Parity mismatch");
    }
}

void prfct_process_random_data_exception() {
    size_t size = sizeof(random_data);
    [[maybe_unused]] size_t error_count = 0;
    
    for (size_t i = 0; i < size / 2; ++i) {
        try {
            prfct_swap_chars_exception(random_data[i], random_data[size - 1 - i]);
        } catch (const std::runtime_error&) {
            error_count++;
        }
    }
}

static void BM_process_return_code(benchmark::State& state) {
    initialize_random_data();
    
    for (auto _ : state) {
        prfct_process_random_data_return_code();
        benchmark::DoNotOptimize(random_data);
    }
}

static void BM_process_exception(benchmark::State& state) {
    initialize_random_data();
    
    for (auto _ : state) {
        prfct_process_random_data_exception();
        benchmark::DoNotOptimize(random_data);
    }
}

BENCHMARK(BM_process_return_code);
BENCHMARK(BM_process_exception);

BENCHMARK_MAIN();
