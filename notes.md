# Performance Optimization Notes

## Loop Unrolling

**Project**: `ilp_no_data_dependencies`

### Findings
Manual loop unrolling is usually **useless or harmful** at high optimization levels:
- GCC O3: sequential code is faster than any manual unrolling
- Overly aggressive unrolling (16x, 32x) slows down code by 2x due to code bloat and register pressure
- Compiler applies auto-unrolling and vectorization better than manual optimization

---

## Branch Prediction

**Project**: `branch_prediction`

### Findings
Unpredictable branches catastrophically affect performance:
- ~50% branch miss rate → 7-8x slowdown
- Each misprediction = ~20 cycles (pipeline flush)
- Code is identical in both cases — the only difference is pattern predictability

### [[likely]]/[[unlikely]] Attributes
C++20 compiler hints affect code layout optimization:
- **Correct hint** ([[likely]] on predictable ~97%): 9-13% speedup
- **Wrong hint** ([[likely]]/[[unlikely]] on unpredictable ~50%): 3-10% slowdown
- Attributes help only when hint matches reality

---

## ILP - Data Dependencies

**Project**: `ilp_data_dependencies`

### Findings
Data dependencies between loop iterations prevent CPU from exploiting parallelism:
- GCC O3: Independent 200ms vs Dependent 800ms → **4x slowdown**
- Dependency chain (prev_avg) forces sequential execution
- CPU must wait for iteration N-1 to complete before starting iteration N

---

## Cache Locality

**Project**: `cache_locality`

### Findings
Memory access patterns strongly impact performance due to cache misses and hardware prefetching.

---

## Virtual Functions

**Project**: `virtual`

### Findings
Virtual functions harm performance by **blocking compiler optimizations**, not by vtable lookup overhead:

- **Clang O3**: Virtual version 10x slower (154 µs vs 1449 µs)
  - Cannot inline virtual function through vtable
  - Cannot vectorize loop with virtual calls
  - Uses scalar SSE instead of packed SIMD

- **GCC O3**: Virtual version **faster** (115 µs vs 197 µs)
  - Performs **devirtualization** when concrete type is known
  - Generates identical vectorized code for both versions
  - Shows that devirtualization is possible when compiler sees concrete type

**Key takeaway**: Real cost of virtual functions is prevented optimizations (inlining + vectorization), not the 1-2 instruction vtable lookup.
