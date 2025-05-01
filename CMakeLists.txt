cmake_minimum_required(VERSION 3.10.0)
project(cpp-practice VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
add_library(cpp-practice matrix_ops.cpp modern_cpp.cpp graph.cpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  cpp-practice-test
  tests.cpp
  test_graph.cpp
)
target_link_libraries(
  cpp-practice-test
  cpp-practice
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cpp-practice-test)
