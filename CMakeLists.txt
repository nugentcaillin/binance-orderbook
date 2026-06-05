cmake_minimum_required(VERSION 3.23)

project(BinanceOrderBook)


# External dependencies
include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.12.0
)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY git@github.com:gabime/spdlog.git
  GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(nlohmann_json)
FetchContent_MakeAvailable(spdlog)

find_package(Boost REQUIRED)

if(BUILD_TESTING)
  enable_testing()
endif()

# Dependencies
add_subdirectory(order_book)
add_subdirectory(poll_socket)

add_executable(BinanceOrderBook)

target_sources(BinanceOrderBook
	PRIVATE
  src/main.cpp
)

target_link_libraries(BinanceOrderBook
  PRIVATE
  nlohmann_json::nlohmann_json
  spdlog::spdlog
  OrderBook::OrderBook
  PollSocket::PollSocket
  Boost::headers
)
