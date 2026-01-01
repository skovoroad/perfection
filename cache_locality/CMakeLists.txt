# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Include common configuration
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/PerfectionCommon.cmake)

# Project name and version
project(cache_locality VERSION 1.0)

# Setup project with common configuration
perfection_setup_project(cache_locality)
