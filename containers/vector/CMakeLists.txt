# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Include common configuration  
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/PerfectionCommon.cmake)

# Project name and version
project(vector_benchmarks VERSION 1.0)

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

# Common libraries
set(COMMON_LIBS
    ${BENCHMARK_BUILD_DIR}/src/libbenchmark.a
    ${ABSEIL_BUILD_DIR}/absl/container/libabsl_raw_hash_set.a
    ${ABSEIL_BUILD_DIR}/absl/container/libabsl_hashtablez_sampler.a
    pthread
    rt
)

# bench_insert
add_executable(bench_insert bench_insert.cpp)
target_include_directories(bench_insert PRIVATE ${COMMON_INCLUDES})
target_link_libraries(bench_insert PRIVATE ${COMMON_LIBS})

# bench_copy
add_executable(bench_copy bench_copy.cpp)
target_include_directories(bench_copy PRIVATE ${COMMON_INCLUDES})
target_link_libraries(bench_copy PRIVATE ${COMMON_LIBS})

# bench_iterate
add_executable(bench_iterate bench_iterate.cpp)
target_include_directories(bench_iterate PRIVATE ${COMMON_INCLUDES})
target_link_libraries(bench_iterate PRIVATE ${COMMON_LIBS})