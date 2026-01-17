#include <random>
#include <benchmark/benchmark.h>

// Size: 1 million floats = 4MB array
constexpr size_t ARRAY_SIZE = 1024 * 1024;
static float data[ARRAY_SIZE];

void initialize_data() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(1.0f, 100.0f);
    
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        data[i] = dis(gen);
    }
}

// Non-virtual class with regular method
class NonVirtualTransform {
public:
    float prfct_transform(float x) {
        return x * 1.5f + 0.1f;
    }
};

// Virtual interface
class IVirtualTransform {
public:
    virtual ~IVirtualTransform() = default;
    virtual float prfct_transform(float x) = 0;
};

// Virtual implementation with SAME logic as non-virtual
class VirtualTransformImpl : public IVirtualTransform {
public:
    float prfct_transform(float x) override {
        return x * 1.5f + 0.1f;
    }
};

// Process through NON-VIRTUAL method call
void prfct_process_nonvirtual(float* arr, size_t size) {
    NonVirtualTransform t;
    for (size_t i = 0; i < size; ++i) {
        arr[i] = t.prfct_transform(arr[i]);
    }
}

// Process through VIRTUAL method call
void prfct_process_virtual(float* arr, size_t size) {
    VirtualTransformImpl impl;
    IVirtualTransform* t = &impl;
    for (size_t i = 0; i < size; ++i) {
        arr[i] = t->prfct_transform(arr[i]);
    }
}

static void BM_nonvirtual(benchmark::State& state) {
    initialize_data();
    
    for (auto _ : state) {
        prfct_process_nonvirtual(data, ARRAY_SIZE);
        benchmark::DoNotOptimize(data);
    }
}

static void BM_virtual(benchmark::State& state) {
    initialize_data();
    
    for (auto _ : state) {
        prfct_process_virtual(data, ARRAY_SIZE);
        benchmark::DoNotOptimize(data);
    }
}

BENCHMARK(BM_nonvirtual);
BENCHMARK(BM_virtual);

BENCHMARK_MAIN();
