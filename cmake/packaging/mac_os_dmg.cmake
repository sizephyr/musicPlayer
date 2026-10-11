cmake_minimum_required(VERSION 3.16)

set_target_properties(
  musicPlayer
  PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "musicPlayer"
             MACOSX_BUNDLE_GUI_IDENTIFIER "com.sizephyr.music_player"
             MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
             MACOSX_BUNDLE_SHORT_VERSION_STRING
             "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

qt_generate_deploy_qml_app_script(TARGET musicPlayer OUTPUT_SCRIPT
                                  deploy_script)

install(SCRIPT ${deploy_script})

set(CPACK_PACKAGE_NAME "music player")
set(CPACK_PACKAGE_VENDOR "sizephyr")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_GENERATOR "DragNDrop")

set(CPACK_SET_DESTDIR ON)
