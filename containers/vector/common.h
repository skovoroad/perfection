#pragma once

#include <array>

// =============================================================================
// Common Data Structures for Container Benchmarks
// =============================================================================

// Small item: 4 bytes
using SmallItem = int;

// Medium item: 24 bytes
struct MediumItem {
    double x, y, z;
    MediumItem() : x(0), y(0), z(0) {}
    explicit MediumItem(int val) : x(val), y(val), z(val) {}
};

// Large item: 256 bytes
struct LargeItem {
    std::array<int, 64> data;
    LargeItem() { data.fill(0); }
    explicit LargeItem(int val) { data.fill(val); }
};
