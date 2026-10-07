cmake_minimum_required(VERSION 3.16)
project(Configuration LANGUAGES CXX)

qt_policy(SET QTP0001 NEW)

add_subdirectory(themes)

set_source_files_properties(Configuration.qml PROPERTIES QT_QML_SINGLETON_TYPE
                                                         TRUE)

qt_add_qml_module(
  configuration
  URI
  "modules.configuration"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/modules/configuration"
  STATIC
  QML_FILES
  Configuration.qml
  SOURCES
  configurationbase.h
  configurationbase.cpp)

option(MAKE_DEFAULT_CONFIG "Собрать конфигурацию по-умолчанию" OFF)

include(${CMAKE_SOURCE_DIR}/cmake/make_default_application_config.cmake)

target_link_libraries(configuration PRIVATE themesplugin common_lib)

target_include_directories(configurationplugin
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
