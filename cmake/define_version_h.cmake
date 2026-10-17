cmake_minimum_required(VERSION 3.16)

execute_process(
  COMMAND ${GIT_EXECUTABLE} log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

if(NOT GIT_COMMIT_HASH)
  set(GIT_COMMIT_HASH "NONE")
endif()

message(STATUS "GIT_COMMIT_HASH - ${GIT_COMMIT_HASH}")

set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(RELEASE_CHANNEL "alpha.closed") # закрытая альфа. Других пока что нет)

configure_file("${CMAKE_SOURCE_DIR}/cmake/res/version.h.in"
               "${CMAKE_BINARY_DIR}/version.h")
