cmake_minimum_required(VERSION 3.16)
project(common_lib LANGUAGES CXX)

qt_add_library(
  common_lib
  STATIC
  utils/utils.h
  utils/utils.cpp
  utils/singleton.h
  entityid.h
  stringentityid.h
  httpcode.h
  httpcode.cpp
  httpheader.h
  httpheader.cpp
  filemanagebackend.h
  filemanagebackend.cpp
  mp3description.h
  mp3description.cpp)

qt_add_qml_module(
  common_lib
  URI
  "modules.common.lib"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/modules/common/lib"
  SOURCES
  statuslabel.h)

target_link_libraries(
  common_lib
  PRIVATE Qt6::Core Qt6::Quick Qt6::Multimedia QCoro6::Core
          # TagLib::TagLib
          ZLIB::ZLIB)
target_include_directories(common_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(common_lib PRIVATE "${CMAKE_BINARY_DIR}")
