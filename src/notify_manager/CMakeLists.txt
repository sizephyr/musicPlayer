cmake_minimum_required(VERSION 3.16)
project(notify_manager LANGUAGES CXX)

qt_policy(SET QTP0001 NEW)
find_package(Qt6 REQUIRED COMPONENTS Core Quick)

set_source_files_properties(notifymanagermodel.h
                            PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(
  notify_manager
  URI
  "modules.notify_manager"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/modules/notify_manager"
  STATIC
  QML_FILES
  NotifyManager.qml
  Notify.qml
  SOURCES
  notifymanagernotifiesviewmodel.h
  notifymanagernotifiesviewmodel.cpp
  notifymanagermodel.h
  notifymanagermodel.cpp
  QML_FILES
  NotifyLastTimeIndicator.qml)

target_link_libraries(
  notify_manager PRIVATE Qt6::Core Qt6::Gui Qt6::Quick Qt6::Qml commonplugin
                         configurationplugin)

target_include_directories(notify_managerplugin
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(
  TARGETS notify_managerplugin
  BUNDLE DESTINATION .
  ARCHIVE DESTINATION musicPlayer.app/Contents/Frameworks
  LIBRARY DESTINATION musicPlayer.app/Contents/Frameworks)
