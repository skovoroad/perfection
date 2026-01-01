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

// Non-virtual swapper class
class NoVirtualSwapper {
public:
    void prfct_swap(char& a, char& b) {
        std::swap(a, b);
    }
};

// Pure virtual interface
class VirtualSwapper {
public:
    virtual ~VirtualSwapper() = default;
    virtual void prfct_swap(char& a, char& b) = 0;
};

// Implementation of virtual interface
class VirtualSwapperImpl : public VirtualSwapper {
public:
    virtual void prfct_swap(char& a, char& b) override {
        std::swap(a, b);
    }
};

void prfct_process_data_nonvirtual() {
    NoVirtualSwapper swapper;
    size_t size = sizeof(random_data);
    for (size_t i = 0; i < size / 2; ++i) {
        swapper.prfct_swap(random_data[i], random_data[size - 1 - i]);
    }
}

void prfct_process_data_virtual() {
    VirtualSwapperImpl impl;
    VirtualSwapper* swapper = &impl;
    size_t size = sizeof(random_data);
    for (size_t i = 0; i < size / 2; ++i) {
        swapper->prfct_swap(random_data[i], random_data[size - 1 - i]);
    }
}

static void BM_nonvirtual(benchmark::State& state) {
    initialize_random_data();
    
    for (auto _ : state) {
        prfct_process_data_nonvirtual();
        benchmark::DoNotOptimize(random_data);
    }
}

static void BM_virtual(benchmark::State& state) {
    initialize_random_data();
    
    for (auto _ : state) {
        prfct_process_data_virtual();
        benchmark::DoNotOptimize(random_data);
    }
}

BENCHMARK(BM_nonvirtual);
BENCHMARK(BM_virtual);

BENCHMARK_MAIN();
