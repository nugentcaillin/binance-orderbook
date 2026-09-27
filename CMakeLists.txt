cmake_minimum_required(VERSION 3.23)

project(binance-orderbook)

add_executable(dummy)
target_sources(dummy
    PRIVATE
        examples/dummy/main.cpp
)
