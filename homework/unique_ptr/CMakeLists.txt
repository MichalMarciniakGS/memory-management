cmake_minimum_required(VERSION 3.10)
project(unique_ptr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/homework/unique_ptr)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)

# Create library target for unique_ptr
add_library(unique_ptr INTERFACE)
target_include_directories(unique_ptr INTERFACE ${CMAKE_SOURCE_DIR}/homework/unique_ptr)

# Create test executable
add_executable(unique_ptr_tests unique_ptr_tests.cpp)
target_link_libraries(unique_ptr_tests PRIVATE unique_ptr gtest gtest_main)

# Enable testing
enable_testing()
add_test(NAME unique_ptr_tests COMMAND unique_ptr_tests)