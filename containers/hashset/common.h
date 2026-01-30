#pragma once

// =============================================================================
// Common Data Structures for Hash Set Container Benchmarks
// =============================================================================

// Set sizes
constexpr int SMALL_SET_SIZE = 16;
constexpr int MEDIUM_SET_SIZE = 256;
constexpr int LARGE_SET_SIZE = 4096;

// Element type (trivial hash)
using Element = int;
