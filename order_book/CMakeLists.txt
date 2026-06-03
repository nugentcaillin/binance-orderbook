cmake_minimum_required(VERSION 3.23)

project(OrderBook)

# External dependencies
# avoid building tests in dependencies
set(BUILD_TESTING_BACKUP ${BUILD_TESTING})
set(BUILD_TESTING OFF CACHE BOOL "Disable testing of external dependencies" FORCE)
include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(spdlog)
# restore BUILD_TESTING
set(BUILD_TESTING ${BUILD_TESTING_BACKUP} CACHE BOOL "" FORCE)

add_library(OrderBook)
add_library(OrderBook::OrderBook ALIAS OrderBook)

target_sources(OrderBook
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS
    include
  FILES
    include/order_book/foo.hpp
)

target_link_libraries(OrderBook
  PRIVATE
  spdlog::spdlog
)


