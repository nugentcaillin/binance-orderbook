cmake_minimum_required(VERSION 3.23)

project(PollSocket)

add_library(PollSocket INTERFACE)
add_library(PollSocket::PollSocket ALIAS PollSocket)
