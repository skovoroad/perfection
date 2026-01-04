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

// Independent: each iteration is completely independent
// CPU can execute multiple iterations in parallel (ILP)
void prfct_swap_independent() {
    for (size_t i = 0; i < ARRAY_SIZE / 2; i++) {
        int a = data[i];
        int b = data[ARRAY_SIZE - 1 - i];
        int avg = (a + b) / 2;  // average of CURRENT pair
        
        std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        data[i] += avg;
        data[ARRAY_SIZE - 1 - i] += avg;
    }
}

// Dependent: each iteration depends on the previous one
// prev_avg creates a dependency chain preventing parallel execution
// CPU must wait for iteration N-1 to complete before starting iteration N
void prfct_swap_dependent() {
    int prev_avg = 0;
    for (size_t i = 0; i < ARRAY_SIZE / 2; i++) {
        std::swap(data[i], data[ARRAY_SIZE - 1 - i]);
        data[i] += prev_avg;  // depends on previous iteration!
        data[ARRAY_SIZE - 1 - i] += prev_avg;
        
        // Compute average for NEXT iteration (creates dependency)
        prev_avg = (data[i] + data[ARRAY_SIZE - 1 - i]) / 2;
    }
}

static void BM_independent(benchmark::State& state) {
    initialize_data();
    
    for (auto _ : state) {
        prfct_swap_independent();
        benchmark::DoNotOptimize(data);
    }
}

static void BM_dependent(benchmark::State& state) {
    initialize_data();
    
    for (auto _ : state) {
        prfct_swap_dependent();
        benchmark::DoNotOptimize(data);
    }
}

BENCHMARK(BM_independent);
BENCHMARK(BM_dependent);

BENCHMARK_MAIN();
