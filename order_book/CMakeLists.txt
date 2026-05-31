cmake_minimum_required(VERSION 3.23)

project(OrderBook)

add_library(OrderBook INTERFACE)
add_library(OrderBook::OrderBook ALIAS OrderBook)


