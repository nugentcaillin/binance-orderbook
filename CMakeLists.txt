cmake_minimum_required(VERSION 3.23)

project(binance-orderbook)

add_library(libbinance)
set_target_properties(libbinance PROPERTIES PREFIX "")

target_sources(libbinance
    PRIVATE
        libbinance/libbinance/hello.cpp 
    PUBLIC 
        FILE_SET HEADERS
        BASE_DIRS 
            libbinance  
        FILES 
            libbinance/libbinance/hello.hpp
)


add_executable(dummy)
target_sources(dummy
    PRIVATE
        examples/dummy/main.cpp
)
target_link_libraries(dummy
    PRIVATE 
        libbinance
)
