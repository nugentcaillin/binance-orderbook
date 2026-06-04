cmake_minimum_required(VERSION 3.23)
project(PollSocketTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# External dependencies
# avoid building tests in dependencies
set(BUILD_TESTING_BACKUP ${BUILD_TESTING})
set(BUILD_TESTING OFF CACHE BOOL "Disable testing of external dependencies" FORCE)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
# restore BUILD_TESTING
set(BUILD_TESTING ${BUILD_TESTING_BACKUP} CACHE BOOL "" FORCE)


add_executable(
  PollSocketTests
  ring_buffer.cpp
)
target_link_libraries(
  PollSocketTests
  GTest::gtest_main
  PollSocket::PollSocket
)

include(GoogleTest)
gtest_discover_tests(PollSocketTests)
