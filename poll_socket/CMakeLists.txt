cmake_minimum_required(VERSION 3.23)

project(PollSocket)

# External dependencies
include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(spdlog)

add_library(PollSocket)
add_library(PollSocket::PollSocket ALIAS PollSocket)


target_sources(PollSocket
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS
    include
  FILES
    include/poll_socket/foo.hpp
)

target_link_libraries(PollSocket
  PRIVATE
  spdlog::spdlog
)
