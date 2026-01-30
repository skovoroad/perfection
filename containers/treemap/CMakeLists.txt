# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Include common configuration  
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/PerfectionCommon.cmake)

# Project name and version
project(treemap_benchmarks VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Option to set optimization level
set(OPTIMIZATION_LEVEL "O2" CACHE STRING "Optimization level")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -${OPTIMIZATION_LEVEL}")
message(STATUS "Using optimization level: -${OPTIMIZATION_LEVEL}")

# Third-party paths
get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
set(THIRDPARTY_DIR "${PROJECT_ROOT_DIR}/3rdparty")
set(BENCHMARK_BUILD_DIR "${THIRDPARTY_DIR}/.build/benchmark")
set(BENCHMARK_SRC_DIR "${THIRDPARTY_DIR}/src/benchmark")
set(BOOST_SRC_DIR "${THIRDPARTY_DIR}/src/boost")
set(ABSEIL_SRC_DIR "${THIRDPARTY_DIR}/src/abseil-cpp")
set(ABSEIL_BUILD_DIR "${THIRDPARTY_DIR}/.build/abseil")

# Common include directories
set(COMMON_INCLUDES
    ${BENCHMARK_SRC_DIR}/include
    ${BOOST_SRC_DIR}/libs/container/include
    ${BOOST_SRC_DIR}/libs/config/include
    ${BOOST_SRC_DIR}/libs/assert/include
    ${BOOST_SRC_DIR}/libs/static_assert/include
    ${BOOST_SRC_DIR}/libs/type_traits/include
    ${BOOST_SRC_DIR}/libs/core/include
    ${BOOST_SRC_DIR}/libs/move/include
    ${BOOST_SRC_DIR}/libs/intrusive/include
    ${ABSEIL_SRC_DIR}
)

# All required abseil libraries for btree_map
set(COMMON_LIBS
    ${BENCHMARK_BUILD_DIR}/src/libbenchmark.a
    # Container libs
    ${ABSEIL_BUILD_DIR}/absl/container/libabsl_raw_hash_set.a
    ${ABSEIL_BUILD_DIR}/absl/container/libabsl_hashtablez_sampler.a
    # Hash libs
    ${ABSEIL_BUILD_DIR}/absl/hash/libabsl_hash.a
    ${ABSEIL_BUILD_DIR}/absl/hash/libabsl_city.a
    # Synchronization (for Mutex)
    ${ABSEIL_BUILD_DIR}/absl/synchronization/libabsl_synchronization.a
    ${ABSEIL_BUILD_DIR}/absl/synchronization/libabsl_graphcycles_internal.a
    ${ABSEIL_BUILD_DIR}/absl/synchronization/libabsl_kernel_timeout_internal.a
    # Time
    ${ABSEIL_BUILD_DIR}/absl/time/libabsl_time.a
    ${ABSEIL_BUILD_DIR}/absl/time/libabsl_time_zone.a
    ${ABSEIL_BUILD_DIR}/absl/time/libabsl_civil_time.a
    # Debugging (for stacktrace)
    ${ABSEIL_BUILD_DIR}/absl/debugging/libabsl_stacktrace.a
    ${ABSEIL_BUILD_DIR}/absl/debugging/libabsl_borrowed_fixup_buffer.a
    ${ABSEIL_BUILD_DIR}/absl/debugging/libabsl_symbolize.a
    ${ABSEIL_BUILD_DIR}/absl/debugging/libabsl_examine_stack.a
    ${ABSEIL_BUILD_DIR}/absl/debugging/libabsl_debugging_internal.a
    ${ABSEIL_BUILD_DIR}/absl/debugging/libabsl_demangle_internal.a
    ${ABSEIL_BUILD_DIR}/absl/debugging/libabsl_demangle_rust.a
    ${ABSEIL_BUILD_DIR}/absl/debugging/libabsl_decode_rust_punycode.a
    ${ABSEIL_BUILD_DIR}/absl/debugging/libabsl_utf8_for_code_point.a
    # Profiling
    ${ABSEIL_BUILD_DIR}/absl/profiling/libabsl_exponential_biased.a
    # Strings
    ${ABSEIL_BUILD_DIR}/absl/strings/libabsl_strings.a
    ${ABSEIL_BUILD_DIR}/absl/strings/libabsl_strings_internal.a
    ${ABSEIL_BUILD_DIR}/absl/strings/libabsl_str_format_internal.a
    # Numeric
    ${ABSEIL_BUILD_DIR}/absl/numeric/libabsl_int128.a
    # Base libs
    ${ABSEIL_BUILD_DIR}/absl/base/libabsl_base.a
    ${ABSEIL_BUILD_DIR}/absl/base/libabsl_spinlock_wait.a
    ${ABSEIL_BUILD_DIR}/absl/base/libabsl_malloc_internal.a
    ${ABSEIL_BUILD_DIR}/absl/base/libabsl_raw_logging_internal.a
    ${ABSEIL_BUILD_DIR}/absl/base/libabsl_throw_delegate.a
    ${ABSEIL_BUILD_DIR}/absl/base/libabsl_tracing_internal.a
    pthread
    rt
    dl
)

# bench_insert
add_executable(bench_insert bench_insert.cpp)
target_include_directories(bench_insert PRIVATE ${COMMON_INCLUDES})
target_link_libraries(bench_insert PRIVATE ${COMMON_LIBS})

# bench_lookup
add_executable(bench_lookup bench_lookup.cpp)
target_include_directories(bench_lookup PRIVATE ${COMMON_INCLUDES})
target_link_libraries(bench_lookup PRIVATE ${COMMON_LIBS})

# bench_iterate
add_executable(bench_iterate bench_iterate.cpp)
target_include_directories(bench_iterate PRIVATE ${COMMON_INCLUDES})
target_link_libraries(bench_iterate PRIVATE ${COMMON_LIBS})

# bench_erase
add_executable(bench_erase bench_erase.cpp)
target_include_directories(bench_erase PRIVATE ${COMMON_INCLUDES})
target_link_libraries(bench_erase PRIVATE ${COMMON_LIBS})
