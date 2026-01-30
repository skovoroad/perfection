#pragma once

// =============================================================================
// Common Data Structures for Hash Map Container Benchmarks
// =============================================================================

// Map sizes
constexpr int SMALL_MAP_SIZE = 16;
constexpr int MEDIUM_MAP_SIZE = 256;
constexpr int LARGE_MAP_SIZE = 4096;

// Key type (trivial hash)
using Key = int;

// Value type (trivially copyable, small)
using Value = int;
