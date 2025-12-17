# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyMinimalProject VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Google Benchmark setup
set(BENCHMARK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../benchmark")

# Check if Google Benchmark exists, if not clone and build it
if(NOT EXISTS "${BENCHMARK_DIR}")
    message(STATUS "Google Benchmark not found in ${BENCHMARK_DIR}")
    message(STATUS "Cloning Google Benchmark...")
    
    execute_process(
        COMMAND git clone https://github.com/google/benchmark.git "${BENCHMARK_DIR}"
        RESULT_VARIABLE GIT_RESULT
    )
    
    if(NOT GIT_RESULT EQUAL "0")
        message(FATAL_ERROR "Failed to clone Google Benchmark")
    endif()
    
    message(STATUS "Building Google Benchmark...")
    file(MAKE_DIRECTORY "${BENCHMARK_DIR}/build")
    
    execute_process(
        COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_DOWNLOAD_DEPENDENCIES=ON ..
        WORKING_DIRECTORY "${BENCHMARK_DIR}/build"
        RESULT_VARIABLE CMAKE_RESULT
    )
    
    if(NOT CMAKE_RESULT EQUAL "0")
        message(FATAL_ERROR "Failed to configure Google Benchmark")
    endif()
    
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build . --config Release
        WORKING_DIRECTORY "${BENCHMARK_DIR}/build"
        RESULT_VARIABLE BUILD_RESULT
    )
    
    if(NOT BUILD_RESULT EQUAL "0")
        message(FATAL_ERROR "Failed to build Google Benchmark")
    endif()
    
    message(STATUS "Google Benchmark built successfully")
else()
    message(STATUS "Found Google Benchmark at ${BENCHMARK_DIR}")
endif()

# Add Google Benchmark
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests")
add_subdirectory("${BENCHMARK_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/benchmark" EXCLUDE_FROM_ALL)

# Add the executable
add_executable(MyMinimalProject main.cpp)

# Link Google Benchmark
target_link_libraries(MyMinimalProject PRIVATE benchmark::benchmark)