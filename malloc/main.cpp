#include <thread>
#include <vector>
#include <string>
#include <benchmark/benchmark.h>

// Number of allocations per thread
constexpr size_t NUM_ALLOCATIONS = 1'000;

// Helper function containing common benchmark logic
static void RunMallocContentionBenchmark(benchmark::State& state, int num_threads, size_t allocation_size) {
    for (auto _ : state) {
        std::vector<std::thread> threads;
        std::vector<std::vector<void*>> all_ptrs(num_threads);
        
        // Reserve to avoid reallocation during benchmark
        for (int t = 0; t < num_threads; ++t) {
            all_ptrs[t].reserve(NUM_ALLOCATIONS);
        }
        
        // Create threads - each does identical work
        for (int t = 0; t < num_threads; ++t) {
            threads.emplace_back([&, t]() {
                for (size_t i = 0; i < NUM_ALLOCATIONS; ++i) {
                    void* ptr = ::operator new(allocation_size);
                    all_ptrs[t].push_back(ptr);
                    benchmark::DoNotOptimize(ptr);
                }
            });
        }
        
        // Wait for all threads
        for (auto& thread : threads) {
            thread.join();
        }
        
        // Deallocation OUTSIDE measurements
        state.PauseTiming();
        for (auto& ptrs : all_ptrs) {
            for (void* ptr : ptrs) {
                ::operator delete(ptr);
            }
        }
        state.ResumeTiming();
    }
    
    // Report total bytes allocated
    state.SetBytesProcessed(
        state.iterations() * num_threads * NUM_ALLOCATIONS * allocation_size
    );
}

// 64B allocation benchmarks
template<int NumThreads>
static void BM_Size64B(benchmark::State& state) {
    RunMallocContentionBenchmark(state, NumThreads, 64);
}

// 4KB allocation benchmarks
template<int NumThreads>
static void BM_Size4KB(benchmark::State& state) {
    RunMallocContentionBenchmark(state, NumThreads, 4096);
}

// Register benchmarks with hierarchical naming: BM_<Size>/<ThreadCount>
// Groups by allocation size in summary.md, showing all thread counts together

// 64 bytes allocation size
BENCHMARK(BM_Size64B<1>)->Name("BM_Size64B/Thread1");
BENCHMARK(BM_Size64B<2>)->Name("BM_Size64B/Thread2");
BENCHMARK(BM_Size64B<4>)->Name("BM_Size64B/Thread4");
BENCHMARK(BM_Size64B<8>)->Name("BM_Size64B/Thread8");

// 4KB allocation size
BENCHMARK(BM_Size4KB<1>)->Name("BM_Size4KB/Thread1");
BENCHMARK(BM_Size4KB<2>)->Name("BM_Size4KB/Thread2");
BENCHMARK(BM_Size4KB<4>)->Name("BM_Size4KB/Thread4");
BENCHMARK(BM_Size4KB<8>)->Name("BM_Size4KB/Thread8");

BENCHMARK_MAIN();
