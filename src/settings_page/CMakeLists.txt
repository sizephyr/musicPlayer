cmake_minimum_required(VERSION 3.16)
project(settings_page LANGUAGES CXX)

qt_policy(SET QTP0001 NEW)
qt_add_qml_module(
  settings_page
  URI
  "modules.settings_page"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/modules/settings_page"
  STATIC
  QML_FILES
  SettingsPage.qml
  SectionItem.qml)

target_link_libraries(settings_page PRIVATE Qt6::Gui Qt6::Quick commonplugin
                                            common_lib configurationplugin)

target_include_directories(settings_pageplugin
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(
  TARGETS settings_pageplugin
  BUNDLE DESTINATION .
  ARCHIVE DESTINATION musicPlayer.app/Contents/Frameworks
  LIBRARY DESTINATION musicPlayer.app/Contents/Frameworks)
