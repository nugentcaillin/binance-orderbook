cmake_minimum_required(VERSION 3.23)

project(PollSocket)


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

add_library(PollSocket)
add_library(PollSocket::PollSocket ALIAS PollSocket)


target_sources(PollSocket
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS
    include
  FILES
    include/poll_socket/ring_buffer.hpp
)

target_link_libraries(PollSocket
  PRIVATE
  spdlog::spdlog
)

#build tests
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
