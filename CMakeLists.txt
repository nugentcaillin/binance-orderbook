cmake_minimum_required(VERSION 3.23)

project(BinanceOrderBook)


# External dependencies
add_subdirectory(extern/json)
add_subdirectory(extern/spdlog)

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
)
