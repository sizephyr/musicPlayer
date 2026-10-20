cmake_minimum_required(VERSION 3.16)

set(QT_DEPLOY_TRANSLATIONS OFF)
qt_generate_deploy_qml_app_script(TARGET musicPlayer OUTPUT_SCRIPT
                                  deploy_script)

install(SCRIPT ${deploy_script})

set(CPACK_GENERATOR "AppImage")
set(CPACK_PACKAGE_NAME "music player")
set(CPACK_PACKAGE_VENDOR "sizephyr")
set(CPACK_APPIMAGE_DESKTOP_FILE "cmake/res/com.sizephyr.playerApp.desktop")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

install(FILES "cmake/res/com.sizephyr.playerApp.desktop"
        DESTINATION share/applications)
install(FILES "img/player_icon.png"
        DESTINATION share/icons/hicolor/256x256/apps)

set(CPACK_SET_DESTDIR ON)
