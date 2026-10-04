cmake_minimum_required(VERSION 3.16)
project(Configuration LANGUAGES CXX)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.9.0)
FetchContent_MakeAvailable(yaml-cpp)

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

target_link_libraries(configuration PRIVATE themesplugin yaml-cpp::yaml-cpp
                                            common_lib)

target_include_directories(configurationplugin
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
