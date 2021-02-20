cmake_minimum_required(VERSION 3.6) 
project(floton VERSION 0.0.1)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

if(NOT "${CMAKE_CXX_STANDARD}")
  set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ standard to be used")
endif(NOT "${CMAKE_CXX_STANDARD}")

include( CheckCXXSourceCompiles )
include( CheckIncludeFileCXX )
include( CheckIncludeFile )

# Checking for C++11 headers
CHECK_INCLUDE_FILE_CXX("atomic" CPP_HAS_ATOMICS)
CHECK_INCLUDE_FILE_CXX("thread" CPP_HAS_THREADS)
CHECK_INCLUDE_FILE_CXX("cstdint" CPP_HAS_STDINT)

if(NOT CPP_HAS_ATOMICS)
    message(FATAL_ERROR "atomics C++ header is not found, this is required for floton")
endif(NOT CPP_HAS_ATOMICS)

if(NOT CPP_HAS_THREADS)
    message(FATAL_ERROR "thread C++ header is not found, this is required for floton")
endif(NOT CPP_HAS_THREADS)

# Checking for SIMD headers
CHECK_INCLUDE_FILE("x86intrin.h" C_HAS_X86_INTRIN)
if(NOT C_HAS_X86_INTRIN)
     message(WARNING "X86 intrinsics not found, performance may degrade")
endif(NOT C_HAS_X86_INTRIN)

include ( TestBigEndian )
TEST_BIG_ENDIAN(FEATURE_IS_BIG_ENDIAN)
if (FEATURE_IS_BIG_ENDIAN)
    message(status "Detected big endian platform")
    add_definitions(-DFLOTON_ENDIAN_BIG)
else(FEATURE_IS_BIG_ENDIAN)
    message(status "Detected little endian platform")
    add_definitions(-DFLOTON_ENDIAN_LITTLE)
endif(FEATURE_IS_BIG_ENDIAN)

# options
option(WITH_testing "To build and run tests" ON)

if(WITH_testing MATCHES ON)
    enable_testing()
endif(WITH_testing MATCHES ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)