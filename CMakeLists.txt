cmake_minimum_required(VERSION 3.16)

project(
  musicPlayer
  VERSION 0.1.4
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(QML_IMPORT_PATH
    "${CMAKE_BINARY_DIR}/src/qml"
    CACHE PATH "Path to application qml modules" FORCE)

include(FetchContent)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia)
# find_package(TagLib REQUIRED)
find_package(ZLIB REQUIRED)
include(cmake/define_version_h.cmake)
include(cmake/fetch_qcoro.cmake)

qt_standard_project_setup(REQUIRES 6.11)

option(SHOW_RESOURCES "Показать ресурсы проекта в лог" OFF)

add_subdirectory(src)
add_subdirectory(src/configuration_management)
add_subdirectory(src/common)
add_subdirectory(src/core)
add_subdirectory(src/page_manager)
add_subdirectory(src/notify_manager)
add_subdirectory(src/main_page)
add_subdirectory(src/settings_page)

qt_add_executable(musicPlayer MACOSX_BUNDLE src/main.cpp)

if(SHOW_RESOURCES)
  target_compile_definitions(musicPlayer PRIVATE SHOW_RESOURCES)
endif()

target_link_libraries(
  musicPlayer PRIVATE Qt6::Quick mainplugin common_lib coreplugin
                      page_managerplugin notify_managerplugin)

set_target_properties(
  musicPlayer
  PROPERTIES # MACOSX_BUNDLE_GUI_IDENTIFIER com.example.musicplayer
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
             MACOSX_BUNDLE TRUE
             WIN32_EXECUTABLE TRUE
             QML_IMPORT_PATH "${CMAKE_BINARY_DIR}/src/qml")

install(
  TARGETS musicPlayer
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(APPLE)
  include(cmake/packaging/mac_os_dmg.cmake)
endif()
include(CPack)
