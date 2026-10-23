cmake_minimum_required(VERSION 3.16)
project(Main LANGUAGES CXX)

qt_policy(SET QTP0001 NEW)

qt_add_qml_module(
  main
  URI
  "Main"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/Main"
  STATIC
  QML_FILES
  Main.qml
  BackgroundImage.qml
  SOURCES
  macoswindowhelper.h
  macoswindowhelper.cpp
  logger.h
  logger.cpp
  RESOURCES
  ../img/base_background.jpg)

target_link_libraries(main PRIVATE Qt6::Quick common_lib themesplugin
                                   configurationplugin settings_pageplugin)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(
  TARGETS mainplugin
  BUNDLE DESTINATION .
  ARCHIVE DESTINATION musicPlayer.app/Contents/Frameworks
  LIBRARY DESTINATION musicPlayer.app/Contents/Frameworks)
