cmake_minimum_required(VERSION 3.10)
project(SharedPtrProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#compile options
add_compile_options(-Wall -Wextra -Werror -pedantic)

#include header files
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
add_executable(shared_ptr_tests shared_ptr/shared_ptr_tests.cpp)
add_executable(weak_ptr_tests shared_ptr/weak_ptr_tests.cpp)

#Link GTest
target_link_libraries(shared_ptr_tests GTest::gtest GTest::gtest_main)
target_link_libraries(weak_ptr_tests GTest::gtest GTest::gtest_main)

enable_testing()

add_test(NAME SharedPtrTests COMMAND shared_ptr_tests)
add_test(NAME WeakPtrTests COMMAND weak_ptr_tests)