cmake_minimum_required(VERSION 3.16)
project(common_lib LANGUAGES CXX)

qt_add_library(
  common_lib
  STATIC
  value_objects/namecomponent.h
  value_objects/firstname.h
  value_objects/firstname.cpp
  value_objects/patronymic.h
  value_objects/patronymic.cpp
  value_objects/lastname.h
  value_objects/lastname.cpp
  value_objects/fullname.h
  value_objects/fullname.cpp
  value_objects/sex.h
  sexvalueobject.h
  sexvalueobject.cpp
  value_objects/timeslot.h
  value_objects/timeslot.cpp
  value_objects/begindatetimepoint.h
  value_objects/begindatetimepoint.cpp
  value_objects/enddatetimepoint.h
  value_objects/enddatetimepoint.cpp
  utils/utils.h
  utils/utils.cpp
  utils/singleton.h
  entityid.h
  stringentityid.h
  httpcode.h
  httpcode.cpp
  httpheader.h
  httpheader.cpp)

qt_add_qml_module(
  common_lib
  URI
  "modules.common.lib"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/modules/common/lib"
  SOURCES
  statuslabel.h
  filemanagebackend.h
  filemanagebackend.cpp)

target_link_libraries(common_lib PRIVATE Qt6::Core Qt6::Quick Qt6::Multimedia
                                         TagLib::TagLib ZLIB::ZLIB)
target_include_directories(common_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(common_lib PRIVATE "${CMAKE_BINARY_DIR}")
