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

    # Third-party dependencies setup
    set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty")
    set(THIRDPARTY_SRC_DIR "${THIRDPARTY_DIR}/src")
    set(THIRDPARTY_BUILD_DIR "${THIRDPARTY_DIR}/.build")

    # Google Benchmark setup
    set(BENCHMARK_SRC_DIR "${THIRDPARTY_SRC_DIR}/benchmark")
    set(BENCHMARK_BUILD_DIR "${THIRDPARTY_BUILD_DIR}/benchmark")

    # Check if Google Benchmark exists, if not clone and build it
    if(NOT EXISTS "${BENCHMARK_SRC_DIR}")
        message(STATUS "Google Benchmark not found in ${BENCHMARK_SRC_DIR}")
        message(STATUS "Cloning Google Benchmark...")
        
        execute_process(
            COMMAND git clone https://github.com/google/benchmark.git "${BENCHMARK_SRC_DIR}"
            RESULT_VARIABLE GIT_RESULT
        )
        
        if(NOT GIT_RESULT EQUAL "0")
            message(FATAL_ERROR "Failed to clone Google Benchmark")
        endif()
    else()
        message(STATUS "Found Google Benchmark at ${BENCHMARK_SRC_DIR}")
    endif()

    # Build Google Benchmark if not already built
    if(NOT EXISTS "${BENCHMARK_BUILD_DIR}/src/libbenchmark.a")
        message(STATUS "Building Google Benchmark...")
        file(MAKE_DIRECTORY "${BENCHMARK_BUILD_DIR}")
        
        execute_process(
            COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release 
                    -DBENCHMARK_DOWNLOAD_DEPENDENCIES=ON 
                    -DBENCHMARK_ENABLE_TESTING=OFF
                    "${BENCHMARK_SRC_DIR}"
            WORKING_DIRECTORY "${BENCHMARK_BUILD_DIR}"
            RESULT_VARIABLE CMAKE_RESULT
        )
        
        if(NOT CMAKE_RESULT EQUAL "0")
            message(FATAL_ERROR "Failed to configure Google Benchmark")
        endif()
        
        execute_process(
            COMMAND ${CMAKE_COMMAND} --build . --config Release
            WORKING_DIRECTORY "${BENCHMARK_BUILD_DIR}"
            RESULT_VARIABLE BUILD_RESULT
        )
        
        if(NOT BUILD_RESULT EQUAL "0")
            message(FATAL_ERROR "Failed to build Google Benchmark")
        endif()
        
        message(STATUS "Google Benchmark built successfully")
    else()
        message(STATUS "Google Benchmark already built at ${BENCHMARK_BUILD_DIR}")
    endif()

    # Boost setup (header-only)
    set(BOOST_SRC_DIR "${THIRDPARTY_SRC_DIR}/boost")
    
    if(NOT EXISTS "${BOOST_SRC_DIR}")
        message(STATUS "Boost not found in ${BOOST_SRC_DIR}")
        message(STATUS "Cloning Boost...")
        
        execute_process(
            COMMAND git clone --depth 1 --branch boost-1.84.0 https://github.com/boostorg/boost.git "${BOOST_SRC_DIR}"
            RESULT_VARIABLE GIT_RESULT
        )
        
        if(NOT GIT_RESULT EQUAL "0")
            message(FATAL_ERROR "Failed to clone Boost")
        endif()
        
        message(STATUS "Boost cloned successfully (header-only, no build required)")
    else()
        message(STATUS "Found Boost at ${BOOST_SRC_DIR}")
    endif()

    # Abseil setup
    set(ABSEIL_SRC_DIR "${THIRDPARTY_SRC_DIR}/abseil-cpp")
    set(ABSEIL_BUILD_DIR "${THIRDPARTY_BUILD_DIR}/abseil")
    
    if(NOT EXISTS "${ABSEIL_SRC_DIR}")
        message(STATUS "Abseil not found in ${ABSEIL_SRC_DIR}")
        message(STATUS "Cloning Abseil...")
        
        execute_process(
            COMMAND git clone https://github.com/abseil/abseil-cpp.git "${ABSEIL_SRC_DIR}"
            RESULT_VARIABLE GIT_RESULT
        )
        
        if(NOT GIT_RESULT EQUAL "0")
            message(FATAL_ERROR "Failed to clone Abseil")
        endif()
    else()
        message(STATUS "Found Abseil at ${ABSEIL_SRC_DIR}")
    endif()

    # Build Abseil if not already built
    if(NOT EXISTS "${ABSEIL_BUILD_DIR}/absl")
        message(STATUS "Building Abseil...")
        file(MAKE_DIRECTORY "${ABSEIL_BUILD_DIR}")
        
        execute_process(
            COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release 
                    -DCMAKE_CXX_STANDARD=17
                    -DABSL_ENABLE_INSTALL=ON
                    "${ABSEIL_SRC_DIR}"
            WORKING_DIRECTORY "${ABSEIL_BUILD_DIR}"
            RESULT_VARIABLE CMAKE_RESULT
        )
        
        if(NOT CMAKE_RESULT EQUAL "0")
            message(FATAL_ERROR "Failed to configure Abseil")
        endif()
        
        execute_process(
            COMMAND ${CMAKE_COMMAND} --build . --config Release
            WORKING_DIRECTORY "${ABSEIL_BUILD_DIR}"
            RESULT_VARIABLE BUILD_RESULT
        )
        
        if(NOT BUILD_RESULT EQUAL "0")
            message(FATAL_ERROR "Failed to build Abseil")
        endif()
        
        message(STATUS "Abseil built successfully")
    else()
        message(STATUS "Abseil already built at ${ABSEIL_BUILD_DIR}")
    endif()

    # Add Google Benchmark
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests")
    add_subdirectory("${BENCHMARK_SRC_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/benchmark" EXCLUDE_FROM_ALL)

    # Add the executable
    add_executable(${PROJECT_NAME} main.cpp)

    # Link Google Benchmark
    target_link_libraries(${PROJECT_NAME} PRIVATE benchmark::benchmark)
endfunction()
