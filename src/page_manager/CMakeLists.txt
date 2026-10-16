cmake_minimum_required(VERSION 3.16)
project(page_manager LANGUAGES CXX)

qt_policy(SET QTP0001 NEW)

set_source_files_properties(pagemanager.h PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(
  page_manager
  URI
  "modules.page_manager"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/modules/page_manager"
  STATIC
  SOURCES
  pagemanager.h
  pagemanager.cpp
  pagetype.h
  ipagecontext.h
  ipagecontext.cpp
  pagecontextmodel.h
  pagecontextmodel.cpp
  simplepagecontextwithmodel.h)

target_link_libraries(
  page_manager PRIVATE Qt6::Core Qt6::Gui Qt6::Quick Qt6::Qml commonplugin
                       configurationplugin)

target_include_directories(page_managerplugin
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(
  TARGETS page_managerplugin
  BUNDLE DESTINATION .
  ARCHIVE DESTINATION musicPlayer.app/Contents/Frameworks
  LIBRARY DESTINATION musicPlayer.app/Contents/Frameworks)
