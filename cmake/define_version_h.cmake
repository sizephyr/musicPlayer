cmake_minimum_required(VERSION 3.16)

configure_file("${CMAKE_SOURCE_DIR}/cmake/res/version.h.in"
               "${CMAKE_BINARY_DIR}/version.h")
