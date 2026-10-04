cmake_minimum_required(VERSION 3.16)
project(core LANGUAGES CXX)

qt_policy(SET QTP0001 NEW)

qt_add_qml_module(
  core
  URI
  "modules.core"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/modules/core"
  STATIC
  SOURCES
  core.cpp
  core.h)

target_link_libraries(
  core
  PRIVATE Qt6::Gui
          Qt6::Quick
          yaml-cpp::yaml-cpp
          commonplugin
          configurationplugin
          common_lib
          page_managerplugin
          auth_pageplugin)
