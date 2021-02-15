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

# Checking for C++11 headers
CHECK_INCLUDE_FILE_CXX("atomic" CPP_HAS_ATOMICS)
CHECK_INCLUDE_FILE_CXX("thread" CPP_HAS_THREADS)

if(NOT CPP_HAS_ATOMICS)
    message(SEND_ERROR "atomics C++ header is not found, this is required for floton")
endif(NOT CPP_HAS_ATOMICS)

if(NOT CPP_HAS_THREADS)
    message(SEND_ERROR "thread C++ header is not found, this is required for floton")
endif(NOT CPP_HAS_THREADS)

# options
option(WITH_testing "To build and run tests" ON)

if(WITH_testing MATCHES ON)
    enable_testing()
endif(WITH_testing MATCHES ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)