cmake_minimum_required(VERSION 3.16)
project(Themes LANGUAGES CXX)

qt_policy(SET QTP0001 NEW)

qt_add_qml_module(
  themes
  URI
  "modules.themes"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/modules/themes"
  STATIC
  QML_FILES
  ThemeProvider.qml)
