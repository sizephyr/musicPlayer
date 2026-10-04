cmake_minimum_required(VERSION 3.16)
project(common LANGUAGES CXX)

qt_policy(SET QTP0001 NEW)

add_subdirectory(lib)

qt_add_qml_module(
  common
  URI
  "modules.common"
  VERSION
  1.0
  OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/src/qml/modules/common"
  STATIC
  QML_FILES
  BaseActionBackgroundRect.qml
  BaseButton.qml
  ShadowedRectangle.qml
  CloseButton.qml
  ProfileAvatar.qml
  Separator.qml
  AppendButton.qml
  SearchBar.qml
  StatusLabel.qml
  BaseTextInput.qml
  BaseTextArea.qml


  SOURCES
  basebuttonstyle.h
  pageclosingcontext.h
  RESOURCES
  img/female_pic.png
  img/male_pic.png
  img/unknown_sex_icon_pic.png
  img/append_pic.png
  img/search_bar_magnifier_pic.png)

target_link_libraries(common PRIVATE Qt6::Gui Qt6::Quick configurationplugin
                                     common_lib)

target_include_directories(commonplugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
