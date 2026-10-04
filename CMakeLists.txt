cmake_minimum_required(VERSION 3.16)

project(
  musicPlayer
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(QML_IMPORT_PATH
    "${CMAKE_BINARY_DIR}/src/qml"
    CACHE PATH "Path to application qml modules" FORCE)
include(FetchContent)

find_package(Qt6 REQUIRED COMPONENTS Quick)
include(cmake/define_version_h.cmake)

qt_standard_project_setup(REQUIRES 6.11)

if(APPLE)
  message(
    STATUS
      "Detected Apple platform. Enabling experimental library features for std::chrono..."
  )

  add_compile_options(-fexperimental-library)
else()
  message(STATUS "Non-Apple platform. Standard compilation flags applied.")
endif()

add_subdirectory(src)
add_subdirectory(src/configuration_management)
add_subdirectory(src/common)
add_subdirectory(src/core)
add_subdirectory(src/page_manager)
add_subdirectory(src/header)
add_subdirectory(src/notify_manager)
# add_subdirectory(src/first_configuration_page)
add_subdirectory(src/auth_page)

qt_add_executable(musicPlayer src/main.cpp)

target_link_libraries(
  musicPlayer
  PRIVATE Qt6::Quick
          mainplugin
          headerplugin
          coreplugin
          page_managerplugin
          notify_managerplugin
          auth_pageplugin)

install(
  TARGETS musicPlayer
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
