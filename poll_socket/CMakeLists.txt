cmake_minimum_required(VERSION 3.23)

project(PollSocket)


# External dependencies
# avoid building tests in dependencies
set(BUILD_TESTING_BACKUP ${BUILD_TESTING})
set(BUILD_TESTING OFF CACHE BOOL "Disable testing of external dependencies" FORCE)
include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY git@github.com:gabime/spdlog.git
  GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(spdlog)
set(BUILD_TESTING ${BUILD_TESTING_BACKUP} CACHE BOOL "" FORCE)


find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(PollSocket)
add_library(PollSocket::PollSocket ALIAS PollSocket)


target_sources(PollSocket
  PRIVATE
    src/socket_listener.cpp
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS
    include
  FILES
    include/poll_socket/ring_buffer.hpp
    include/poll_socket/socket_listener.hpp
)

target_link_libraries(PollSocket
  PUBLIC
  Boost::headers
  PRIVATE
  OpenSSL::SSL
  spdlog::spdlog
)

#build tests
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
