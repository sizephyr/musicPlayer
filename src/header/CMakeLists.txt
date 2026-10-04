cmake_minimum_required(VERSION 3.16)
project(header LANGUAGES CXX)

qt_policy(SET QTP0001 NEW)
find_package(Qt6 REQUIRED COMPONENTS Core Quick)

set_source_files_properties(headermodel.h PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(
  header
  URI
  "modules.header"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/modules/header"
  STATIC
  QML_FILES
  Header.qml
  HeaderNotifyLabel.qml
  ApplicationNameLabel.qml
  HeaderMenu.qml
  SOURCES
  headermodel.h
  headermodel.cpp
  RESOURCES
  pics/correct_devices.png
  pics/wrong_devices.png
  pics/settings.png
  pics/ellipse.png)

target_link_libraries(header PRIVATE Qt6::Core Qt6::Gui Qt6::Quick Qt6::Qml
                                     commonplugin configurationplugin)

target_include_directories(headerplugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
