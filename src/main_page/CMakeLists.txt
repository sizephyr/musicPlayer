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
  PlayListItem.qml
  SomeMediaPlayer.qml
  OpenCurrentFolderBtn.qml
  SOURCES
  mainpagemodel.h
  mainpagemodel.cpp
  playlistmodel.h
  playlistmodel.cpp
  playcontextmodel.h
  playcontextmodel.cpp
  RESOURCES
  ../../img/open-file.png
  ../../img/unknown_icon_pic.png)

target_link_libraries(
  main_page
  PRIVATE Qt6::Gui
          Qt6::Quick
          Qt6::Qml
          Qt6::Multimedia
          QCoro6::Core
          commonplugin
          common_lib
          configurationplugin
          page_managerplugin)

target_include_directories(main_pageplugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
