cmake_minimum_required(VERSION 3.10)
project(SharedPtrProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include of shared_ptr.hpp
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#GTest include
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

#Test executable
add_executable(shared_ptr_tests shared_ptr_tests.cpp)

#Link GTest
target_link_libraries(shared_ptr_tests gtest gtest_main)

enable_testing()

add_test(NAME SharedPtrTests COMMAND shared_ptr_tests)