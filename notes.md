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
