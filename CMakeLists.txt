cmake_minimum_required(VERSION 3.15)
project(KnightsTourSolver VERSION 1.0.0 LANGUAGES CXX)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Compiler-specific flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC or Clang
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG -flto")
elseif(MSVC)
    # Visual Studio
    set(CMAKE_CXX_FLAGS_DEBUG "/W4 /Od /Zi /DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG /GL")
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files (will be populated as we build)
set(SOURCES
    src/main.cpp
)

# Create executable
add_executable(knights_tour ${SOURCES})

# Enable Link Time Optimization for Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET knights_tour PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Print build configuration
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
