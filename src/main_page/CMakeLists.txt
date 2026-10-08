cmake_minimum_required(VERSION 3.16)
project(main_page LANGUAGES CXX)

qt_policy(SET QTP0001 NEW)
qt_add_qml_module(
  main_page
  URI
  "modules.main_page"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/modules/main_page"
  STATIC
  QML_FILES
  MainPage.qml
  PlayProgressBar.qml
  PlayList.qml
  PlayContext.qml
  ActiveDirectoryDialog.qml
  SOURCES
  mainpagemodel.h
  mainpagemodel.cpp
  playlistmodel.h
  playlistmodel.cpp
  SOURCES
  playcontextmodel.h
  playcontextmodel.cpp
  QML_FILES PlayListItem.qml
  QML_FILES SomeMediaPlayer.qml)

target_link_libraries(
  main_page
  PRIVATE Qt6::Gui
          Qt6::Quick
          Qt6::Qml
          commonplugin
          common_lib
          configurationplugin
          page_managerplugin)

target_include_directories(main_pageplugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
