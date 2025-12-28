# Common CMake configuration for Perfection framework projects
# This file should be included at the beginning of each project's CMakeLists.txt

# Option to select compiler (gcc or clang)
set(COMPILER_CHOICE "gcc" CACHE STRING "Choose compiler: gcc or clang")
set_property(CACHE COMPILER_CHOICE PROPERTY STRINGS gcc clang)

# Set the compiler based on the choice
if(COMPILER_CHOICE STREQUAL "gcc")
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    message(STATUS "Using GCC compiler")
elseif(COMPILER_CHOICE STREQUAL "clang")
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
    message(STATUS "Using Clang compiler")
else()
    message(FATAL_ERROR "Invalid compiler choice: ${COMPILER_CHOICE}. Use 'gcc' or 'clang'")
endif()

# Function to setup a benchmark project
# Usage: perfection_setup_project(project_name)
function(perfection_setup_project PROJECT_NAME)
    # Set the C++ standard
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED True)

    # Option to set optimization level
    set(OPTIMIZATION_LEVEL "O2" CACHE STRING "Optimization level: O0, O1, O2, O3, Os, Ofast")
    set_property(CACHE OPTIMIZATION_LEVEL PROPERTY STRINGS O0 O1 O2 O3 Os Ofast)

    # Apply optimization level
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -${OPTIMIZATION_LEVEL}" PARENT_SCOPE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -${OPTIMIZATION_LEVEL}" PARENT_SCOPE)
    message(STATUS "Using optimization level: -${OPTIMIZATION_LEVEL}")

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
    add_executable(${PROJECT_NAME} main.cpp)

    # Link Google Benchmark
    target_link_libraries(${PROJECT_NAME} PRIVATE benchmark::benchmark)
endfunction()
