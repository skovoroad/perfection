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

// Pure virtual interface
class Swapper {
public:
    virtual ~Swapper() = default;
    virtual void swap(char& a, char& b) = 0;
};

// Implementation with noexcept
class NoExceptSwapper : public Swapper {
public:
    virtual void swap(char& a, char& b) noexcept override {
        std::swap(a, b);
    }
};

// Implementation without noexcept
class RegularSwapper : public Swapper {
public:
    virtual void swap(char& a, char& b) override {
        std::swap(a, b);
    }
};

void process_data_noexcept() {
    NoExceptSwapper impl;
    Swapper* swapper = &impl;
    size_t size = sizeof(random_data);
    for (size_t i = 0; i < size / 2; ++i) {
        swapper->swap(random_data[i], random_data[size - 1 - i]);
    }
}

void process_data_regular() {
    RegularSwapper impl;
    Swapper* swapper = &impl;
    size_t size = sizeof(random_data);
    for (size_t i = 0; i < size / 2; ++i) {
        swapper->swap(random_data[i], random_data[size - 1 - i]);
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
