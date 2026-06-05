cmake_minimum_required(VERSION 3.23)

project(PollSocket)


# External dependencies
# avoid building tests in dependencies
set(BUILD_TESTING_BACKUP ${BUILD_TESTING})
set(BUILD_TESTING OFF CACHE BOOL "Disable testing of external dependencies" FORCE)
include(FetchContent)

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
)

#build tests
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
