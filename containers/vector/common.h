#pragma once

#include <array>

// =============================================================================
// Common Data Structures for Container Benchmarks
// =============================================================================

// Small element: 4 bytes
using SmallElement = int;

// Medium element: 24 bytes
struct Point {
    double x, y, z;
    Point() : x(0), y(0), z(0) {}
    explicit Point(int val) : x(val), y(val), z(val) {}
};

// Large element: 256 bytes
struct LargeStruct {
    std::array<int, 64> data;
    LargeStruct() { data.fill(0); }
    explicit LargeStruct(int val) { data.fill(val); }
};
