cmake_minimum_required(VERSION 3.16)
project(auth_page LANGUAGES CXX)

qt_policy(SET QTP0001 NEW)
qt_add_qml_module(
  auth_page
  URI
  "modules.auth_page"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/modules/auth_page"
  STATIC
  QML_FILES
  AuthPage.qml
  RESOURCES
  img/opened_eye.svg
  img/closed_eye.svg
  img/icl_logo.png
  SOURCES authpagemodel.h authpagemodel.cpp)

target_link_libraries(
  auth_page PRIVATE Qt6::Gui Qt6::Quick Qt6::Qml commonplugin
                    configurationplugin page_managerplugin)

target_include_directories(auth_pageplugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
