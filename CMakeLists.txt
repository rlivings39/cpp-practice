cmake_minimum_required(VERSION 3.10.0)
project(cpp-practice VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
add_library(cpp-practice matrix_ops.cpp modern_cpp.cpp graph.cpp data_structures.cpp pub_sub.cpp)
add_executable(matrix-main matrix_main.cpp)
target_link_libraries(matrix-main cpp-practice)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# include(CheckCXXCompilerFlag)
# CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
# if(COMPILER_SUPPORTS_MARCH_NATIVE)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
# endif()

enable_testing()

add_executable(
  cpp-practice-test
  tests.cpp
  test_graph.cpp
  test_pub_sub.cpp
  leetcode.cpp
)
target_link_libraries(
  cpp-practice-test
  cpp-practice
  GTest::gtest_main
)

add_executable(test-data-structures test_data_structures.cpp)
target_link_libraries(test-data-structures cpp-practice GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(cpp-practice-test)
gtest_discover_tests(test-data-structures)
